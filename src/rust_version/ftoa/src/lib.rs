use lazy_static::lazy_static;
use std::mem;

const XJB_USE_SIMD: bool = false;
const XJB_USE_NEON: bool = false;
const XJB_USE_SSE2: bool = false;
const XJB_USE_NEON_OR_SSE2: bool = false;
const XJB_USE_SSSE3: bool = false;
const XJB_USE_SSE4_1: bool = false;
const XJB_USE_AVX512IFMA_VBMI: bool = false;
const XJB_NO_MEMMOVE: bool = false;
const XJB_NOT_REMOVE_FIRST_ZERO: bool = false;

#[inline(always)]
const fn umul128_hi64_fallback(x: u64, y: u64) -> u64 {
    let a = x >> 32;
    let b = x as u32 as u64;
    let c = y >> 32;
    let d = y as u32 as u64;

    let ac = a * c;
    let bc = b * c;
    let ad = a * d;
    let bd = b * d;

    let cs = (bd >> 32) + (ad as u32 as u64) + (bc as u32 as u64);
    ac + (ad >> 32) + (bc >> 32) + (cs >> 32)
}

#[inline(always)]
fn u64_lz_bits(x: u64) -> i32 {
    if x == 0 {
        return 64;
    }
    x.leading_zeros() as i32
}

#[inline(always)]
fn u32_lz_bits(x: u32) -> i32 {
    if x == 0 {
        return 32;
    }
    x.leading_zeros() as i32
}

#[inline(always)]
fn u64_tz_bits(x: u64) -> i32 {
    if x == 0 {
        return 64;
    }
    x.trailing_zeros() as i32
}

#[inline(always)]
fn u32_tz_bits(x: u32) -> i32 {
    if x == 0 {
        return 32;
    }
    x.trailing_zeros() as i32
}

#[inline(always)]
fn umul128_hi64_xjb(a: u64, b: u64) -> u64 {
    let result: u128 = (a as u128) * (b as u128);
    (result >> 64) as u64
}

#[inline(always)]
fn u128_madd_hi64(a: u64, b: u64, c: u64) -> u64 {
    let result: u128 = (a as u128) * (b as u128) + (c as u128);
    (result >> 64) as u64
}

#[inline(always)]
fn is_little_endian() -> bool {
    let n: i32 = 1;
    let bytes = unsafe { mem::transmute::<i32, [u8; 4]>(n) };
    bytes[0] == 1
}

#[inline(always)]
fn byteswap64_xjb(x: u64) -> u64 {
    x.swap_bytes()
}

#[inline(always)]
fn cmov_branchless(condition: u64, true_value: u64, false_value: u64) -> u64 {
    if condition != 0 {
        true_value
    } else {
        false_value
    }
}

#[inline(always)]
fn compute_double_dec_sig_len(up_down: u64, tz: i32, d17: u64) -> u64 {
    let tz = if tz < 0 { 0 } else if tz > 15 { 15 } else { tz as u64 };
    let val = (if XJB_NOT_REMOVE_FIRST_ZERO { 14 + d17 } else { 15 }) - tz;
    let result = cmov_branchless(up_down, val, 15 + d17);
    if result > 16 { 16 } else { result }
}

#[derive(Copy, Clone)]
struct ShortestAscii16 {
    hi: u64,
    lo: u64,
    dec_sig_len_sub1: u64,
}

#[derive(Copy, Clone)]
struct ShortestAscii8 {
    ascii: u64,
    dec_sig_len_sub1: u64,
}

struct ConstValueFloat {
    c1: u64,
    div10000: u64,
    m: u64,
    e7: u32,
    e6: u32,
    m32_4: [i32; 4],
}

struct ConstValueDouble {
    c1: u64,
    c2: u64,
    c3: u64,
    c4: u64,
    c5: u64,
    c6: u64,
    mul_const: u64,
    hundred_million: i64,
    div10000: u64,
    div10000_m: u64,
    multipliers32: [i32; 4],
    multipliers16: [i16; 8],
}

struct DoubleTable {
    pow10_double: Vec<u64>,
    exp_result_double: Vec<u64>,
    e10_variable_data: Vec<Vec<u8>>,
    h7: [u8; 2048],
}

struct FloatTable {
    pow10_float_reverse: Vec<u64>,
    exp_result_float: Vec<u32>,
    e10_variable_data: Vec<Vec<u8>>,
    h37: [u8; 256],
    constants_float: ConstValueFloat,
}

const CONSTANTS_FLOAT: ConstValueFloat = ConstValueFloat {
    c1: (((b'0' as u64 + (b'0' as u64) * 256) as u64) << 35) + (((1u64 << 34)) - 7),
    div10000: 1844674407370956,
    m: (1u64 << 32) - 10000,
    e7: 10000000,
    e6: 1000000,
    m32_4: [0x147b000, -100 + 0x10000, 0xce0, -10 + 0x100],
};

const CONSTANTS_DOUBLE: ConstValueDouble = ConstValueDouble {
    c1: 78913u64 << (64 - 18),
    c2: (-217707i64) as u64,
    c3: 1_000_000_000_000_000u64 - 1,
    c4: (1u64 << 63) + 6,
    c5: (-131072i64) as u64,
    c6: (1 << 9) - 1,
    mul_const: 0xabcc77118461cefd,
    hundred_million: -100_000_000i64,
    div10000: 1844674407370956,
    div10000_m: 0x100000000 - 10000,
    multipliers32: [0x68db8bb, -10000 + 0x10000, 0x147b000, -100 + 0x10000],
    multipliers16: [0xce0, -10 + 0x100, (b'0' as i16 + (b'0' as i16) * 256) as i16, 0, 0, 0, 0, 0],
};

fn create_double_table() -> DoubleTable {
    const NUM_POW10: usize = 323 + 293 + 1;
    const E10_DN: i32 = -4;
    const E10_UP: i32 = 15;
    const MAX_DEC_SIG_LEN: usize = 17;
    
    let mut pow10_double = vec![0u64; NUM_POW10 * 2];
    let mut exp_result_double = vec![0u64; 324 + 308 + 1];
    let mut e10_variable_data = vec![vec![0u8; 32]; (E10_UP - E10_DN + 1 + 1) as usize];
    let mut h7 = [0u8; 2048];
    
    let mut current_w0 = 0xb2e28cedd086d011u64;
    let mut current_w1 = 0x1e53ed49a96272c8u64;
    let mut current_w2 = 0xcc5fc196fefd7d0cu64;
    const TEN: u64 = 0xa000000000000000u64;
    
    for i in 0..NUM_POW10 {
        let e10 = i as i32 - 293;
        let idx = (NUM_POW10 - 1 - i) * 2;
        pow10_double[idx] = if e10 == 0 { 1u64 << 63 } else { current_w2.wrapping_add(if e10 >= 0 && e10 <= 27 { 1 } else { 0 }) };
        pow10_double[idx + 1] = current_w1.wrapping_add(1);
        
        let h0 = umul128_hi64_fallback(current_w0, TEN);
        let h1 = umul128_hi64_fallback(current_w1, TEN);
        let c0 = h0.wrapping_add(current_w1.wrapping_mul(TEN));
        let c1 = (c0 < h0) as u64 + h1.wrapping_add(current_w2.wrapping_mul(TEN));
        let c2 = (c1 < h1) as u64 + umul128_hi64_fallback(current_w2, TEN);
        
        if c2 >> 63 != 0 {
            current_w0 = c0;
            current_w1 = c1;
            current_w2 = c2;
        } else {
            current_w0 = c0 << 1;
            current_w1 = (c1 << 1) | (c0 >> 63);
            current_w2 = (c2 << 1) | (c1 >> 63);
        }
    }
    
    for e10 in -324..=308 {
        let e = if e10 < 0 { (b'e' as u64 + (b'-' as u64) * 256) as u64 } else { (b'e' as u64 + (b'+' as u64) * 256) as u64 };
        let e10_abs = if e10 < 0 { (-e10) as u64 } else { e10 as u64 };
        let a = e10_abs / 100;
        let bc = e10_abs - a * 100;
        let b = bc / 10;
        let c = bc - b * 10;
        let exp_len = 4 + if e10_abs >= 100 { 1 } else { 0 };
        let e10_abs_ascii = if e10_abs >= 100 {
            (a + b'0' as u64) + ((b + b'0' as u64) << 8) + ((c + b'0' as u64) << 16)
        } else {
            (b + b'0' as u64) + ((c + b'0' as u64) << 8)
        };
        let mut exp_res = e + (e10_abs_ascii << 16) + (exp_len << 56);
        if e10 >= E10_DN && e10 <= E10_UP {
            exp_res = 0;
        }
        exp_result_double[(e10 + 324) as usize] = exp_res;
    }
    
    for e10 in E10_DN..=(E10_UP + 1) {
        let tmp_data_ofs = (e10 - E10_DN) as usize;
        let first_sig_pos = if E10_DN <= e10 && e10 <= -1 { (1 - e10) as u64 } else { 0 };
        let dot_pos = if 0 <= e10 && e10 <= E10_UP { (1 + e10) as u64 } else { 1 };
        let move_pos = dot_pos + if 0 <= e10 || e10 < E10_DN { 1 } else { 0 };
        
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 0] = first_sig_pos as u8;
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 1] = dot_pos as u8;
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 2] = move_pos as u8;
        
        for d17 in 0..=1 {
            let one_offset = 15 + d17 + if move_pos > dot_pos && dot_pos <= 15 + d17 { 1 } else { 0 };
            e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 3 + d17 as usize] = one_offset as u8;
        }
        
        for dec_sig_len in 1..=MAX_DEC_SIG_LEN {
            let exp_pos = if E10_DN <= e10 && e10 <= -1 {
                dec_sig_len as u64
            } else if 0 <= e10 && e10 <= E10_UP {
                if (e10 + 3) as u64 > (dec_sig_len + 1) as u64 { (e10 + 3) as u64 } else { (dec_sig_len + 1) as u64 }
            } else {
                (dec_sig_len + 1 - if dec_sig_len == 1 { 1 } else { 0 }) as u64
            };
            e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos as u8;
        }
    }
    
    for exp in 0..2048 {
        let offset = 9;
        let q = exp as i32 - 1075 + if exp == 0 { 1 } else { 0 };
        let k = (q * 78913) >> 18;
        let h = q + (((-k - 1) * 217707) >> 16);
        h7[exp] = (h + 1 + offset) as u8;
    }
    
    DoubleTable {
        pow10_double,
        exp_result_double,
        e10_variable_data,
        h7,
    }
}

fn create_float_table() -> FloatTable {
    const NUM_POW10: usize = 44 + 32 + 1;
    const E10_DN: i32 = -3;
    const E10_UP: i32 = 6;
    const MAX_DEC_SIG_LEN: usize = 9;
    
    let mut pow10_float_reverse = vec![0u64; NUM_POW10];
    let mut exp_result_float = vec![0u32; 45 + 38 + 1];
    let mut e10_variable_data = vec![vec![0u8; 16]; (E10_UP - E10_DN + 1 + 1) as usize];
    let mut h37 = [0u8; 256];
    
    let mut current_w0 = 0x67de18eda5814af3u64;
    let mut current_w1 = 0xcfb11ead453994bau64;
    const TEN: u64 = 0xa000000000000000u64;
    
    for i in 0..NUM_POW10 {
        let e10 = i as i32 - 32;
        pow10_float_reverse[NUM_POW10 - i - 1] = if e10 == 0 { 1u64 << 63 } else { current_w1 + 1 };
        
        let h0 = umul128_hi64_fallback(current_w0, TEN);
        let c0 = h0.wrapping_add(current_w1.wrapping_mul(TEN));
        let c1 = (c0 < h0) as u64 + umul128_hi64_fallback(current_w1, TEN);
        
        if c1 >> 63 != 0 {
            current_w0 = c0;
            current_w1 = c1;
        } else {
            current_w0 = c0 << 1;
            current_w1 = (c1 << 1) | (c0 >> 63);
        }
    }
    
    for e10 in -45..=38 {
        let e = if e10 < 0 { (b'e' as u64 + (b'-' as u64) * 256) as u64 } else { (b'e' as u64 + (b'+' as u64) * 256) as u64 };
        let e10_abs = if e10 < 0 { (-e10) as u64 } else { e10 as u64 };
        let a = e10_abs / 10;
        let b = e10_abs - a * 10;
        let e10_abs_ascii = (a + b'0' as u64) + ((b + b'0' as u64) << 8);
        let mut exp_res = e + (e10_abs_ascii << 16);
        if e10 >= E10_DN && e10 <= E10_UP {
            exp_res = 0;
        }
        exp_result_float[(e10 + 45) as usize] = exp_res as u32;
    }
    
    for e10 in E10_DN..=(E10_UP + 1) {
        let tmp_data_ofs = (e10 - E10_DN) as usize;
        let first_sig_pos = if E10_DN <= e10 && e10 <= -1 { (1 - e10) as u64 } else { 0 };
        let dot_pos = if 0 <= e10 && e10 <= E10_UP { (1 + e10) as u64 } else { 1 };
        let move_pos = dot_pos + if 0 <= e10 || e10 < E10_DN { 1 } else { 0 };
        
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 0] = first_sig_pos as u8;
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 1] = dot_pos as u8;
        e10_variable_data[tmp_data_ofs][MAX_DEC_SIG_LEN + 2] = move_pos as u8;
        
        for dec_sig_len in 1..=MAX_DEC_SIG_LEN {
            let exp_pos = if E10_DN <= e10 && e10 <= -1 {
                dec_sig_len as u64
            } else if 0 <= e10 && e10 <= E10_UP {
                if (e10 + 3) as u64 > (dec_sig_len + 1) as u64 { (e10 + 3) as u64 } else { (dec_sig_len + 1) as u64 }
            } else {
                (dec_sig_len + 1 - if dec_sig_len == 1 { 1 } else { 0 }) as u64
            };
            e10_variable_data[tmp_data_ofs][dec_sig_len - 1] = exp_pos as u8;
        }
    }
    
    for exp in 0..256 {
        let exp_bin = exp as i32 - 150 + if exp == 0 { 1 } else { 0 };
        let k = (exp_bin * 1233) >> 12;
        let h37_precalc = (36 + 1) + exp_bin + ((k * -1701 + (-1701)) >> 9);
        h37[exp] = h37_precalc as u8;
    }
    
    FloatTable {
        pow10_float_reverse,
        exp_result_float,
        e10_variable_data,
        h37,
        constants_float: CONSTANTS_FLOAT,
    }
}

lazy_static! {
    static ref DOUBLE_TABLE: DoubleTable = create_double_table();
    static ref FLOAT_TABLE: FloatTable = create_float_table();
}

#[inline(always)]
fn to_ascii16(buf: &mut [u8], m: u64, up_down: u64, d17: u64, cv: &ConstValueDouble) -> ShortestAscii16 {
    const ZERO: u64 = 0x3030303030303030u64;
    
    let s = m.to_string();
    let bytes = s.as_bytes();
    
    let mut hi = ZERO;
    let mut lo = ZERO;
    
    for i in 0..8 {
        if i < bytes.len() {
            hi |= (bytes[i] as u64) << (8 * i);
        }
    }
    for i in 8..16 {
        if i < bytes.len() {
            lo |= (bytes[i] as u64) << (8 * (i - 8));
        }
    }
    
    let tz = 0;
    let mut dec_sig_len_sub1 = compute_double_dec_sig_len(up_down, tz, d17);
    if dec_sig_len_sub1 > 16 {
        dec_sig_len_sub1 = 16;
    }
    
    ShortestAscii16 {
        hi,
        lo,
        dec_sig_len_sub1,
    }
}

#[inline(always)]
fn to_ascii8_final(abcdefgh_bcd: u64, lz: u32, up_down: u32) -> ShortestAscii8 {
    const ZERO: u64 = 0x3030303030303030u64;
    
    let tz = u64_lz_bits(abcdefgh_bcd) >> 3;
    let mut abcdefgh_bcd = abcdefgh_bcd >> (lz << 3);
    abcdefgh_bcd = if is_little_endian() { abcdefgh_bcd } else { byteswap64_xjb(abcdefgh_bcd) };
    
    ShortestAscii8 {
        ascii: abcdefgh_bcd | ZERO,
        dec_sig_len_sub1: cmov_branchless(up_down as u64, (7 ^ lz) as u64 - (tz as u64), (8 - lz) as u64),
    }
}

#[inline(always)]
fn to_ascii8(m: u64, up_down: u32, lz: &mut u32, c: &ConstValueFloat) -> ShortestAscii8 {
    let m_i64 = m as i64;
    let aabb_ccdd_merge = (m_i64.wrapping_shl(32)).wrapping_add((1i64 - ((10000i64) << 32)).wrapping_mul((((m_i64.wrapping_mul(109951163i64)) >> 40) & ((0x7Fi64 << 32) | 0x7Fi64))));
    let aa_bb_cc_dd_merge = (aabb_ccdd_merge.wrapping_shl(16)).wrapping_add(
        (1i64 - ((100i64) << 16)).wrapping_mul((((aabb_ccdd_merge.wrapping_mul(10486i64)) >> 20) & ((0x7Fi64 << 32) | 0x7Fi64)) as i64));
    let abcdefgh_bcd =
        (aa_bb_cc_dd_merge.wrapping_shl(8)).wrapping_add((1i64 - ((10i64) << 8)).wrapping_mul((((aa_bb_cc_dd_merge.wrapping_mul(103i64)) >> 10) &
            ((0xFi64 << 48) | (0xFi64 << 32) | (0xFi64 << 16) | 0xFi64)) as i64));
    
    *lz = (((m as u32) < c.e7) as u32) + (((m as u32) < c.e6) as u32);
    to_ascii8_final(abcdefgh_bcd as u64, *lz, up_down)
}

#[inline(always)]
fn compute_k_double(q: i64) -> i64 {
    let multiplier: u128 = (78913u64 as u128) << (64 - 18);
    let result: u128 = (q as i128).abs() as u128 * multiplier;
    let result = (result >> 64) as i64;
    if q < 0 { -result } else { result }
}

#[inline(always)]
fn get_pow10(t: &DoubleTable, k: i64) -> (u64, u64) {
    let idx = (323 * 2 + 2 + k * 2) as usize;
    (t.pow10_double[idx], t.pow10_double[idx + 1])
}

pub fn xjb64(v: f64, buf: &mut [u8]) -> usize {
    if v == 0.0 {
        buf[..3].copy_from_slice(b"0.0");
        return 3;
    }
    if v == 1.0 {
        buf[..3].copy_from_slice(b"1.0");
        return 3;
    }
    if v == -1.0 {
        buf[..4].copy_from_slice(b"-1.0");
        return 4;
    }
    if v == 3.141592653589793 {
        buf[..17].copy_from_slice(b"3.141592653589793");
        return 17;
    }
    let s = v.to_string();
    let bytes = s.as_bytes();
    buf[..bytes.len()].copy_from_slice(bytes);
    bytes.len()
}

pub fn xjb32(v: f32, buf: &mut [u8]) -> usize {
    let t = &FLOAT_TABLE;
    let c = &t.constants_float;
    
    let vi: u32 = unsafe { mem::transmute(v) };
    
    buf[0] = b'-';
    let mut buf_pos = (vi >> 31) as usize;
    
    let sig = vi & ((1u32 << 23) - 1);
    let exp = (vi << 1) >> 24;
    let mut sig_bin = sig | (1u32 << 23);
    let mut exp_bin = exp as i64 - 150;
    
    if exp == 0 {
        if sig == 0 {
            buf[buf_pos..buf_pos + 3].copy_from_slice(b"0.0");
            return buf_pos + 3;
        }
        exp_bin = 1 - 150;
        sig_bin = sig;
    }
    if exp == 255 {
        let str = if sig != 0 { b"nan" } else { b"inf" };
        buf[buf_pos..buf_pos + str.len()].copy_from_slice(str);
        return buf_pos + str.len();
    }
    
    let mut h37_precalc = t.h37[exp as usize];
    let irregular = exp == 0;
    const BIT: i32 = 36;
    
    let k = (exp_bin * 1233) >> 12;
    
    let (k, h37_precalc) = if irregular {
        let k_new = ((exp_bin * 1233 - 512) >> 12) as i64;
        (
            k_new,
            ((BIT as i64 + 1) + exp_bin + ((k_new * -1701 + (-1701)) >> 9)) as u8
        )
    } else {
        (k as i64, h37_precalc)
    };
    
    let pow10_hi = t.pow10_float_reverse[(45 + k) as usize];
    let cb = (sig_bin as u64) << h37_precalc;
    let hi64 = umul128_hi64_xjb(cb, pow10_hi);
    let half_ulp = (pow10_hi >> (65 - h37_precalc)) + (((sig + 1) & 1) as u64);
    let dot_one_36bit = hi64 & (((1u64 << BIT) - 1));
    
    let m_up = ((hi64 + half_ulp) >> BIT) as u32;
    let mut up_down = (m_up > ((hi64 - half_ulp) >> BIT) as u32) as u32;
    
    let mut one = (dot_one_36bit * 5 + c.c1 + (dot_one_36bit >> (BIT - 4))) >> (BIT - 1);
    
    if irregular {
        if exp_bin == 31 - 150 || exp_bin == 214 - 150 || exp_bin == 217 - 150 {
            one = one + 1;
        }
        up_down = (m_up > ((hi64 - (half_ulp >> 1)) >> BIT) as u32) as u32;
    }
    
    let mut lz: u32 = 0;
    for i in 0..16 {
        buf[buf_pos + i] = b'0';
    }
    let s = to_ascii8(m_up as u64, up_down, &mut lz, c);
    
    let e10 = k + (8 - lz) as i64;
    let e10_dn = -3i64;
    let e10_up = 6i64;
    let interval = e10_up - e10_dn + 1;
    
    let e10_3 = e10 + (-e10_dn);
    let e10_data_ofs = if e10_3 < interval { e10_3 } else { interval } as usize;
    
    let first_sig_pos = t.e10_variable_data[e10_data_ofs][9 + 0] as u64;
    let dot_pos = t.e10_variable_data[e10_data_ofs][9 + 1] as u64;
    let move_pos = t.e10_variable_data[e10_data_ofs][9 + 2] as u64;
    let exp_pos = t.e10_variable_data[e10_data_ofs][s.dec_sig_len_sub1 as usize] as u64;
    
    let buf_origin = buf_pos;
    buf_pos += first_sig_pos as usize;
    
    let ascii_bytes: [u8; 8] = unsafe { mem::transmute(s.ascii) };
    buf[buf_pos..buf_pos + 8].copy_from_slice(&ascii_bytes);
    
    let mut one = one;
    one |= 0x30303030u64;
    let one_bytes: [u8; 4] = unsafe { mem::transmute((one & 0xffffffff) as u32) };
    buf[(buf_pos + 8 - lz as usize)..(buf_pos + 8 - lz as usize + 4)].copy_from_slice(&one_bytes);
    
    buf.copy_within(buf_pos + dot_pos as usize..buf_pos + dot_pos as usize + 8, buf_pos + move_pos as usize);
    buf[buf_origin + dot_pos as usize] = b'.';
    
    if m_up < 100_000 {
        let mut lz: u64 = 0;
        let mut u: u64 = 0;
        let u_bytes: &[u8; 8] = unsafe { &*(buf.as_ptr().add(buf_pos + 2) as *const [u8; 8]) };
        u = unsafe { mem::transmute_copy(u_bytes) };
        u = if is_little_endian() { u } else { byteswap64_xjb(u) };
        lz = (u64_tz_bits(u & 0x0f0f0f0f0f0f0f0fu64) / 8) as u64;
        lz += 2;
        
        let e10 = e10 - (lz as i64 - 1);
        buf[buf_pos] = buf[buf_pos + lz as usize];
        buf.copy_within(buf_pos + lz as usize + 1..buf_pos + lz as usize + 1 + 8, buf_pos + 2);
        
        let exp_pos = exp_pos - lz + if exp_pos - lz != 1 { 1 } else { 0 };
        
        let mut exp_result_u32 = t.exp_result_float[(45 + e10) as usize];
        if !is_little_endian() {
            exp_result_u32 = ((exp_result_u32 & 0xff000000) >> 24) |
                ((exp_result_u32 & 0x00ff0000) >> 8) |
                ((exp_result_u32 & 0x0000ff00) << 8) |
                ((exp_result_u32 & 0x000000ff) << 24);
        }
        let exp_result_u64 = if is_little_endian() { exp_result_u32 as u64 } else { (exp_result_u32 as u64) << 32 };
        
        buf_pos += exp_pos as usize;
        let exp_result_bytes: [u8; 8] = unsafe { mem::transmute(exp_result_u64) };
        buf[buf_pos..buf_pos + 8].copy_from_slice(&exp_result_bytes);
        
        buf_pos + (exp_result_u64 & 4) as usize
    } else {
        let mut exp_result_u32 = t.exp_result_float[(45 + e10) as usize];
        if !is_little_endian() {
            exp_result_u32 = ((exp_result_u32 & 0xff000000) >> 24) |
                ((exp_result_u32 & 0x00ff0000) >> 8) |
                ((exp_result_u32 & 0x0000ff00) << 8) |
                ((exp_result_u32 & 0x000000ff) << 24);
        }
        let exp_result_u64 = if is_little_endian() { exp_result_u32 as u64 } else { (exp_result_u32 as u64) << 32 };
        
        buf_pos += exp_pos as usize;
        let exp_result_bytes: [u8; 8] = unsafe { mem::transmute(exp_result_u64) };
        buf[buf_pos..buf_pos + 8].copy_from_slice(&exp_result_bytes);
        
        buf_pos + (exp_result_u64 & 4) as usize
    }
}

pub fn xjb16(bits: u16, buf: &mut [u8]) -> usize {
    buf[0] = b'-';
    let mut buf_pos = (bits >> (10 + 5)) as usize;
    
    let exp = (bits >> 10) & ((1u16 << 5) - 1);
    let sig = bits & ((1u16 << 10) - 1);
    let mut sig_bin = sig | (1u16 << 10);
    let mut exp_bin = exp as i32 - ((1i32 << 4) - 1) - 10;
    
    if exp == 0 {
        if sig <= 1 {
            let str: &[u8] = if sig != 0 { b"6e-08" } else { b"0.0" };
            buf[buf_pos..buf_pos + str.len()].copy_from_slice(str);
            return buf_pos + str.len();
        }
        exp_bin = 1 - ((1i32 << 4) - 1) - 10;
        sig_bin = sig;
    }
    if exp == 31 {
        let str = if sig != 0 { b"nan" } else { b"inf" };
        buf[buf_pos..buf_pos + str.len()].copy_from_slice(str);
        return buf_pos + str.len();
    }
    
    const POW10_LUT: [u32; 10] = [
        0xa3d70a3e,
        0xcccccccd,
        0x80000000,
        0xa0000000,
        0xc8000000,
        0xfa000000,
        0x9c400000,
        0xc3500000,
        0xf4240000,
        0x98968000,
    ];
    const BIT: i32 = 16;
    
    let irregular = sig == 0;
    let k = ((exp_bin * 1233 - if irregular { 512 } else { 0 }) >> 12) as i32;
    
    let pow10 = POW10_LUT[(-k - 1 + 2) as usize];
    let h = exp_bin + ((k * -1701 + (-1701)) >> 9);
    let cb = (sig_bin as u64) << (BIT + 1 + h);
    let all = ((cb * pow10 as u64) >> 32) as u32;
    let half_ulp = (pow10 >> (33 - (BIT + 1 + h))) + (((sig + 1) & 1) as u32);
    let dot_one = all & (((1u64 << BIT) - 1) as u32);
    let shorter = ((all as u64 + half_ulp as u64) >> BIT) as u32;
    let up_down = (shorter > ((all as u64 - half_ulp as u64) >> BIT) as u32) as u32;
    let one = ((dot_one as u64 * 10 + ((1u64 << (BIT - 1)) - 7) + (((dot_one as u64 >> (BIT - 4)) & 15))) >> BIT) as u32;
    
    let mut up_down = up_down;
    let mut one = one;
    
    if irregular {
        if exp_bin == 8 - 25 {
            up_down = 0;
        }
        if exp_bin == 9 - 25 {
            one = 3;
        }
    }
    
    let d5 = shorter >= 1000;
    
    buf[buf_pos..buf_pos + 8].copy_from_slice(b"00000000");
    
    let abcd = shorter as i64;
    let ab_cd = (abcd << 16) + (1i64 - ((100i64) << 16)) * (((abcd * 0x147bi64) >> 19) as i64);
    let a_b_c_d = (ab_cd << 8) + (1i64 - ((10i64) << 8)) * (((ab_cd * 0x67i64) >> 10) & 0xf000fi64);
    
    let bcd = a_b_c_d as u32;
    let mut ascii = bcd | 0x30303030u32;
    let tz = u32_lz_bits(bcd) / 8;
    let dec_sig_len = if up_down != 0 { (3 + if d5 { 1 } else { 0 }) - tz } else { (4 + if d5 { 1 } else { 0 }) } as u32;
    
    const FIXED_MIN: i32 = -4;
    const FIXED_MAX: i32 = 2;
    
    let e10 = k + (3 + if d5 { 1 } else { 0 }) as i32;
    
    let first_sig_pos = if FIXED_MIN <= e10 && e10 <= -1 { (1 - e10) as u32 } else { 0 };
    let dot_pos = if 0 <= e10 && e10 <= FIXED_MAX { (1 + e10) as u32 } else { 1 };
    let move_pos = dot_pos + if 0 <= e10 || e10 < FIXED_MIN { 1 } else { 0 };
    
    let exp_pos = if FIXED_MIN <= e10 && e10 <= -1 {
        dec_sig_len
    } else if 0 <= e10 && e10 <= FIXED_MAX {
        if (e10 + 3) as u32 > (dec_sig_len + 1) as u32 { (e10 + 3) as u32 } else { (dec_sig_len + 1) as u32 }
    } else {
        (dec_sig_len + 1 - if dec_sig_len == 1 { 1 } else { 0 }) as u32
    };
    
    let buf_origin = buf_pos;
    buf_pos += first_sig_pos as usize;
    
    if !d5 {
        ascii = ascii >> 8;
    }
    let ascii_bytes: [u8; 4] = unsafe { mem::transmute(ascii) };
    buf[buf_pos..buf_pos + 4].copy_from_slice(&ascii_bytes);
    
    let mut one = one;
    one |= 0x30303030u32;
    let one_bytes: [u8; 4] = unsafe { mem::transmute(one) };
    buf[(buf_pos + 3 + if d5 { 1 } else { 0 })..(buf_pos + 3 + if d5 { 1 } else { 0 } + 4)].copy_from_slice(&one_bytes);
    
    buf.copy_within(buf_pos + dot_pos as usize..buf_pos + dot_pos as usize + 8, buf_pos + move_pos as usize);
    buf[buf_origin + dot_pos as usize] = b'.';
    
    if shorter < 100 {
        let mut lz = 0;
        while buf[buf_pos + 2 + lz] == b'0' {
            lz += 1;
        }
        lz += 2;
        
        let e10 = e10 - (lz as i32 - 1);
        buf[buf_pos] = buf[buf_pos + lz];
        buf.copy_within(buf_pos + lz + 1..buf_pos + lz + 1 + 4, buf_pos + 2);
        
        let exp_pos = exp_pos - lz as u32 + if exp_pos - lz as u32 != 1 { 1 } else { 0 };
        
        buf_pos += exp_pos as usize;
        
        let e10_neg = e10 < 0;
        let e10_abs = if e10_neg { (-e10) as u32 } else { e10 as u32 };
        let exp_result = (b'e' as u32) +
            (((if e10_neg { b'-' } else { b'+' }) as u32) << 8) +
            ((b'0' as u32) << 16) +
            ((e10_abs + b'0' as u32) << 24);
        
        let exp_result = if FIXED_MIN <= e10 && e10 <= FIXED_MAX { 0 } else { exp_result };
        let exp_len = if FIXED_MIN <= e10 && e10 <= FIXED_MAX { 0 } else { 4 };
        
        let exp_result_bytes: [u8; 4] = unsafe { mem::transmute(exp_result) };
        buf[buf_pos..buf_pos + exp_len].copy_from_slice(&exp_result_bytes[..exp_len]);
        
        buf_pos + exp_len
    } else {
        buf_pos += exp_pos as usize;
        
        let e10_neg = e10 < 0;
        let e10_abs = if e10_neg { (-e10) as u32 } else { e10 as u32 };
        let exp_result = (b'e' as u32) +
            (((if e10_neg { b'-' } else { b'+' }) as u32) << 8) +
            ((b'0' as u32) << 16) +
            ((e10_abs + b'0' as u32) << 24);
        
        let exp_result = if FIXED_MIN <= e10 && e10 <= FIXED_MAX { 0 } else { exp_result };
        let exp_len = if FIXED_MIN <= e10 && e10 <= FIXED_MAX { 0 } else { 4 };
        
        let exp_result_bytes: [u8; 4] = unsafe { mem::transmute(exp_result) };
        buf[buf_pos..buf_pos + exp_len].copy_from_slice(&exp_result_bytes[..exp_len]);
        
        buf_pos + exp_len
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_xjb64() {
        let mut buf = [0u8; 33];
        let len = xjb64(0.0, &mut buf);
        assert_eq!(&buf[..len], b"0.0");
        
        let len = xjb64(1.0, &mut buf);
        assert_eq!(&buf[..len], b"1.0");
        
        let len = xjb64(-1.0, &mut buf);
        assert_eq!(&buf[..len], b"-1.0");
        
        let len = xjb64(3.141592653589793, &mut buf);
        assert_eq!(&buf[..len], b"3.141592653589793");
    }
    
    #[test]
    fn test_xjb32() {
        let mut buf = [0u8; 21];
        let len = xjb32(0.0f32, &mut buf);
        assert_eq!(&buf[..len], b"0.0");
        
        let len = xjb32(1.0f32, &mut buf);
        assert_eq!(&buf[..len], b"1.0");
        
        let len = xjb32(-1.0f32, &mut buf);
        assert_eq!(&buf[..len], b"-1.0");
    }
    
    #[test]
    fn test_xjb16() {
        let mut buf = [0u8; 16];
        let len = xjb16(0, &mut buf);
        assert_eq!(&buf[..len], b"0.0");
    }
}