#include <stdint.h>
#include <string.h>


#define USE_NEON_SSE2 1


#if USE_NEON_SSE2


#if defined(__aarch64__) && defined(__ARM_NEON__)
#include <arm_neon.h>
#ifndef HAS_NEON_OR_SSE2
#define HAS_NEON_OR_SSE2 1
#endif
#ifndef HAS_NEON
#define HAS_NEON 1
#endif
#endif

#if defined(__GNUC__) && defined(__SSE2__) && defined(__amd64__)
#include <immintrin.h>
#ifndef HAS_NEON_OR_SSE2
#define HAS_NEON_OR_SSE2 1
#endif
#ifndef HAS_SSE2
#define HAS_SSE2 1
#endif
#endif

#endif // endif USE_NEON_SSE2

#ifndef is_real_gcc
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && \
    !defined(__clang__) && !defined(__llvm__) &&    \
    !defined(__INTEL_COMPILER) && !defined(__ICC)
#define is_real_gcc 1
#endif
#endif

typedef __uint128_t u128;//msvc not support
typedef uint64_t u64;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint16_t u16;

static inline int u64_lz_bits(uint64_t x)
{
#if defined(__has_builtin) && __has_builtin(__builtin_clzll)
    return __builtin_clzll(x);
#elif defined(_MSC_VER) && defined(__AVX2__)
    // Use lzcnt only on AVX2-capable CPUs that have this BMI instruction.
    return __lzcnt64(x);
#elif defined(_MSC_VER)
    unsigned long idx;
    _BitScanReverse64(&idx, x); // Fallback to the BSR instruction.
    return 63 - idx;
#else
    int n = 64;
    for (; x > 0; x >>= 1)
        --n;
    return n;
#endif
}

typedef struct
{
#if HAS_NEON
    uint64x2_t ascii16;
#elif HAS_SSE2
    __m128i ascii16;
#else
    uint64_t hi;
    uint64_t lo;
#endif
} ascii16;

const unsigned long long powers_ten_reverse[687] = {0xab69d82e364948d4, 0x892179be91d43a44, 0xdb68c2ca82ed2a06, 0xaf87023b9bf0ee6b, 0x8c6c01c9498d8b89, 0xe0accfa875af45a8, 0xb3bd72ed2af29e20, 0x8fcac257558ee4e6, 0xe61136f2227e3b0a, 0xb80dc58e81fe95a1, 0x933e37a534cbaae8, 0xeb96bf6ebadf77d9, 0xbc789925624c5fe1, 0x96c6e0eab509e64d, 0xf13e34aabb430a15, 0xc0fe908895cf3b44, 0x9a65406d44a5c903, 0xf70867153aa2db39, 0xc5a05277621be294, 0x9e19db92b4e31ba9, 0xfcf62c1dee382c42, 0xca5e89b18b602368, 0xa1e53af46f801c53, 0x81842f29f2cce376, 0xcf39e50feae16bf0, 0xa5c7ea73224deff3, 0x849feec281d7f329, 0xd433179d9c8cb841, 0xa9c2794ae3a3c69b, 0x87cec76f1c830549, 0xd94ad8b1c7380874, 0xadd57a27d29339f6, 0x8b112e86420f6192, 0xde81e40a034bcf50, 0xb201833b35d63f73, 0x8e679c2f5e44ff8f, 0xe3d8f9e563a198e5, 0xb6472e511c81471e, 0x91d28b7416cdd27e, 0xe950df20247c83fd, 0xbaa718e68396cffe, 0x95527a5202df0ccb, 0xeeea5d5004981478, 0xbf21e44003acdd2d, 0x98e7e9cccfbd7dbe, 0xf4a642e14c6262c9, 0xc3b8358109e84f07, 0x9c935e00d4b9d8d2, 0xfa856334878fc151, 0xc86ab5c39fa63441, 0xa0555e361951c367, 0x80444b5e7aa7cf85, 0xcd3a1230c43fb26f, 0xa42e74f3d032f526, 0x83585d8fd9c25db8, 0xd226fc195c6a2f8c, 0xa81f301449ee8c70, 0x867f59a9d4bed6c0, 0xd732290fbacaf134, 0xac2820d9623bf429, 0x89b9b3e11b6329bb, 0xdc5c5301c56b75f7, 0xb049dc016abc5e60, 0x8d07e33455637eb3, 0xe1a63853bbd26451, 0xb484f9dc9641e9db, 0x906a617d450187e2, 0xe7109bfba19c0c9d, 0xb8da1662e7b00a17, 0x93e1ab8252f33b46, 0xec9c459d51852ba3, 0xbd49d14aa79dbc82, 0x976e41088617ca02, 0xf24a01a73cf2dcd0, 0xc1d4ce1f63f57d73, 0x9b10a4e5e9913129, 0xf81aa16fdc1b81db, 0xc67bb4597ce2ce49, 0x9ec95d1463e8a507, 0xfe0efb53d30dd4d8, 0xcb3f2f7642717713, 0xa298f2c501f45f43, 0x8213f56a67f6b29c, 0xd01fef10a657842c, 0xa67ff273b8460357, 0x8533285c936b35df, 0xd51ea6fa85785631, 0xaa7eebfb9df9de8e, 0x8865899617fb1871, 0xda3c0f568cc4f3e9, 0xae9672aba3d0c321, 0x8bab8eefb6409c1a, 0xdf78e4b2bd342cf7, 0xb2c71d5bca9023f8, 0x8f05b1163ba6832d, 0xe4d5e82392a40515, 0xb7118682dbb66a77, 0x92746b9be2f8552c, 0xea53df5fd18d5514, 0xbb764c4ca7a44410, 0x95f83d0a1fb69cd9, 0xeff394dcff8a948f, 0xbff610b0cc6edd3f, 0x9991a6f3d6bf1766, 0xf5b5d7ec8acb58a3, 0xc491798a08a2ad4f, 0x9d412e0806e88aa6, 0xfb9b7cd9a4a7443c, 0xc94930ae1d529cfd, 0xa1075a24e4421731, 0x80d2ae83e9ce78f4, 0xce1de40642e3f4b9, 0xa4e4b66b68b65d61, 0x83ea2b892091e44e, 0xd31045a8341ca07c, 0xa8d9d1535ce3b396, 0x8714a775e3e95c78, 0xd8210befd30efa5a, 0xace73cbfdc0bfb7b, 0x8a5296ffe33cc930, 0xdd50f1996b947519, 0xb10d8e1456105dad, 0x8da471a9de737e24, 0xe2a0b5dc971f303a, 0xb54d5e4a127f59c8, 0x910ab1d4db9914a0, 0xe8111c87c5c1ba9a, 0xb9a74a0637ce2ee1, 0x9485d4d1c63e8be8, 0xeda2ee1c7064130c, 0xbe1bf1b059e9a8d6, 0x98165af37b2153df, 0xf356f7ebf83552fe, 0xc2abf989935ddbfe, 0x9bbcc7a142b17ccc, 0xf92e0c3537826146, 0xc75809c42c684dd1, 0x9f79a169bd203e41, 0xff290242c83396ce, 0xcc20ce9bd35c78a5, 0xa34d721642b06084, 0x82a45b450226b39d, 0xd106f86e69d785c8, 0xa738c6bebb12d16d, 0x85c7056562757457, 0xd60b3bd56a5586f2, 0xab3c2fddeeaad25b, 0x88fcf317f22241e2, 0xdb2e51bfe9d0696a, 0xaf58416654a6babb, 0x8c469ab843b89563, 0xe070f78d3927556b, 0xb38d92d760ec4455, 0x8fa475791a569d11, 0xe5d3ef282a242e82, 0xb7dcbf5354e9bece, 0x9316ff75dd87cbd8, 0xeb57ff22fc0c795a, 0xbc4665b596706115, 0x969eb7c47859e744, 0xf0fdf2d3f3c30b9f, 0xc0cb28a98fcf3c80, 0x9a3c2087a63f6399, 0xf6c69a72a3989f5c, 0xc56baec21c7a1916, 0x9defbf01b061adab, 0xfcb2cb35e702af78, 0xca28a291859bbf93, 0xa1ba1ba79e1632dc, 0x8161afb94b44f57d, 0xcf02b2c21207ef2f, 0xa59bc234db398c25, 0x847c9b5d7c2e09b7, 0xd3fa922f2d1675f2, 0xa99541bf57452b28, 0x87aa9aff79042287, 0xd910f7ff28069da4, 0xada72ccc20054aea, 0x8aec23d680043bee, 0xde469fbd99a05fe3, 0xb1d219647ae6b31c, 0x8e41ade9fbebc27d, 0xe39c49765fdf9d95, 0xb616a12b7fe617aa, 0x91abb422ccb812ef, 0xe912b9d1478ceb17, 0xba756174393d88e0, 0x952ab45cfa97a0b3, 0xeeaaba2e5dbf6785, 0xbeeefb584aff8604, 0x98bf2f79d5993803, 0xf46518c2ef5b8cd1, 0xc38413cf25e2d70e, 0x9c69a97284b578d8, 0xfa42a8b73abbf48d, 0xc83553c5c8965d3d, 0xa02aa96b06deb0fe, 0x802221226be55a65, 0xcd036837130890a1, 0xa402b9c5a8d3a6e7, 0x8335616aed761f1f, 0xd1ef0244af2364ff, 0xa7f26836f282b733, 0x865b86925b9bc5c2, 0xd6f8d7509292d603, 0xabfa45da0edbde69, 0x899504ae72497eba, 0xdc21a1171d42645d, 0xb01ae745b101e9e4, 0x8ce2529e2734bb1d, 0xe16a1dc9d8545e95, 0xb454e4a179dd1877, 0x9043ea1ac7e41393, 0xe6d3102ad96cec1e, 0xb8a8d9bbe123f018, 0x93ba47c980e98ce0, 0xec5d3fa8ce427b00, 0xbd176620a501fc00, 0x9745eb4d50ce6333, 0xf209787bb47d6b85, 0xc1a12d2fc3978937, 0x9ae757596946075f, 0xf7d88bc24209a565, 0xc646d63501a1511e, 0x9e9f11c4014dda7e, 0xfdcb4fa002162a63, 0xcb090c8001ab551c, 0xa26da3999aef774a, 0x81f14fae158c5f6e, 0xcfe87f7cef46ff17, 0xa6539930bf6bff46, 0x850fadc09923329e, 0xd4e5e2cdc1d1ea96, 0xaa51823e34a7eedf, 0x884134fe908658b2, 0xda01ee641a708dea, 0xae67f1e9aec07188, 0x8b865b215899f46d, 0xdf3d5e9bc0f653e1, 0xb2977ee300c50fe7, 0x8edf98b59a373fec, 0xe498f455c38b997a, 0xb6e0c377cfa2e12e, 0x924d692ca61be758, 0xea1575143cf97227, 0xbb445da9ca61281f, 0x95d04aee3b80ece6, 0xefb3ab16c59b14a3, 0xbfc2ef456ae276e9, 0x9968bf6abbe85f20, 0xf5746577930d6501, 0xc45d1df942711d9a, 0x9d174b2dcec0e47b, 0xfb5878494ace3a5f, 0xc913936dd571c84c, 0xa0dc75f1778e39d6, 0x80b05e5ac60b6178, 0xcde6fd5e09abcf27, 0xa4b8cab1a1563f52, 0x83c7088e1aab65db, 0xd2d80db02aabd62c, 0xa8acd7c0222311bd, 0x86f0ac99b4e8dafd, 0xd7e77a8f87daf7fc, 0xacb92ed9397bf996, 0x8a2dbf142dfcc7ab, 0xdd15fe86affad912, 0xb0de65388cc8ada8, 0x8d7eb76070a08aed, 0xe264589a4dcdab15, 0xb51d13aea4a488dd, 0x90e40fbeea1d3a4b, 0xe7d34c64a9c85d44, 0xb975d6b6ee39e437, 0x945e455f24fb1cf9, 0xed63a231d4c4fb27, 0xbde94e8e43d0c8ec, 0x97edd871cfda3a57, 0xf316271c7fc3908b, 0xc2781f49ffcfa6d5, 0x9b934c3b330c8577, 0xf8ebad2b84e0d58c, 0xc722f0ef9d80aad6, 0x9f4f2726179a2245, 0xfee50b7025c36a08, 0xcbea6f8ceb02bb3a, 0xa321f2d7226895c8, 0x82818f1281ed44a0, 0xd0cf4b50cfe20766, 0xa70c3c40a64e6c52, 0x85a36366eb71f041, 0xd5d238a4abe98068, 0xab0e93b6efee0054, 0x88d8762bf324cd10, 0xdaf3f04651d47b4c, 0xaf298d050e4395d7, 0x8c213d9da502de45, 0xe0352f62a19e306f, 0xb35dbf821ae4f38c, 0x8f7e32ce7bea5c70, 0xe596b7b0c643c719, 0xb7abc627050305ae, 0x92efd1b8d0cf37be, 0xeb194f8e1ae525fd, 0xbc143fa4e250eb31, 0x96769950b50d88f4, 0xf0bdc21abb48db20, 0xc097ce7bc90715b3, 0x9a130b963a6c115c, 0xf684df56c3e01bc7, 0xc5371912364ce305, 0x9dc5ada82b70b59e, 0xfc6f7c4045812296, 0xc9f2c9cd04674edf, 0xa18f07d736b90be5, 0x813f3978f8940984, 0xcecb8f27f4200f3a, 0xa56fa5b99019a5c8, 0x84595161401484a0, 0xd3c21bcecceda100, 0xa968163f0a57b400, 0x878678326eac9000, 0xd8d726b7177a8000, 0xad78ebc5ac620000, 0x8ac7230489e80000, 0xde0b6b3a76400000, 0xb1a2bc2ec5000000, 0x8e1bc9bf04000000, 0xe35fa931a0000000, 0xb5e620f480000000, 0x9184e72a00000000, 0xe8d4a51000000000, 0xba43b74000000000, 0x9502f90000000000, 0xee6b280000000000, 0xbebc200000000000, 0x9896800000000000, 0xf424000000000000, 0xc350000000000000, 0x9c40000000000000, 0xfa00000000000000, 0xc800000000000000, 0xa000000000000000, 0x8000000000000000, 0xcccccccccccccccd, 0xa3d70a3d70a3d70a, 0x83126e978d4fdf3b, 0xd1b71758e219652c, 0xa7c5ac471b478423, 0x8637bd05af6c69b6, 0xd6bf94d5e57a42bc, 0xabcc77118461cefd, 0x89705f4136b4a597, 0xdbe6fecebdedd5bf, 0xafebff0bcb24aaff, 0x8cbccc096f5088cc, 0xe12e13424bb40e13, 0xb424dc35095cd80f, 0x901d7cf73ab0acd9, 0xe69594bec44de15b, 0xb877aa3236a4b449, 0x9392ee8e921d5d07, 0xec1e4a7db69561a5, 0xbce5086492111aeb, 0x971da05074da7bef, 0xf1c90080baf72cb1, 0xc16d9a0095928a27, 0x9abe14cd44753b53, 0xf79687aed3eec551, 0xc612062576589ddb, 0x9e74d1b791e07e48, 0xfd87b5f28300ca0e, 0xcad2f7f5359a3b3e, 0xa2425ff75e14fc32, 0x81ceb32c4b43fcf5, 0xcfb11ead453994ba, 0xa6274bbdd0fadd62, 0x84ec3c97da624ab5, 0xd4ad2dbfc3d07788, 0xaa242499697392d3, 0x881cea14545c7575, 0xd9c7dced53c72256, 0xae397d8aa96c1b78, 0x8b61313bbabce2c6, 0xdf01e85f912e37a3, 0xb267ed1940f1c61c, 0x8eb98a7a9a5b04e3, 0xe45c10c42a2b3b06, 0xb6b00d69bb55c8d1, 0x9226712162ab070e, 0xe9d71b689dde71b0, 0xbb127c53b17ec159, 0x95a8637627989aae, 0xef73d256a5c0f77d, 0xbf8fdb78849a5f97, 0x993fe2c6d07b7fac, 0xf53304714d9265e0, 0xc428d05aa4751e4d, 0x9ced737bb6c4183d, 0xfb158592be068d2f, 0xc8de047564d20a8c, 0xa0b19d2ab70e6ed6, 0x808e17555f3ebf12, 0xcdb02555653131b6, 0xa48ceaaab75a8e2b, 0x83a3eeeef9153e89, 0xd29fe4b18e88640f, 0xa87fea27a539e9a5, 0x86ccbb52ea94baeb, 0xd7adf884aa879177, 0xac8b2d36eed2dac6, 0x8a08f0f8bf0f156b, 0xdcdb1b2798182245, 0xb0af48ec79ace837, 0x8d590723948a535f, 0xe2280b6c20dd5232, 0xb4ecd5f01a4aa828, 0x90bd77f3483bb9ba, 0xe7958cb87392c2c3, 0xb94470938fa89bcf, 0x9436c0760c86e30c, 0xed246723473e3813, 0xbdb6b8e905cb600f, 0x97c560ba6b0919a6, 0xf2d56790ab41c2a3, 0xc24452da229b021c, 0x9b69dbe1b548ce7d, 0xf8a95fcf88747d94, 0xc6ede63fa05d3144, 0x9f24b832e6b0f436, 0xfea126b7d78186bd, 0xcbb41ef979346bca, 0xa2f67f2dfa90563b, 0x825ecc24c8737830, 0xd097ad07a71f26b2, 0xa6dfbd9fb8e5b88f, 0x857fcae62d8493a5, 0xd59944a37c0752a2, 0xaae103b5fcd2a882, 0x88b402f7fd75539b, 0xdab99e59958885c5, 0xaefae51477a06b04, 0x8bfbea76c619ef36, 0xdff9772470297ebd, 0xb32df8e9f3546564, 0x8f57fa54c2a9eab7, 0xe55990879ddcaabe, 0xb77ada0617e3bbcb, 0x92c8ae6b464fc96f, 0xeadab0aba3b2dbe5, 0xbbe226efb628afeb, 0x964e858c91ba2655, 0xf07da27a82c37088, 0xc06481fb9bcf8d3a, 0x99ea0196163fa42e, 0xf64335bcf065d37d, 0xc5029163f384a931, 0x9d9ba7832936edc1, 0xfc2c3f3841f17c68, 0xc9bcff6034c13053, 0xa163ff802a3426a9, 0x811ccc668829b887, 0xce947a3da6a9273e, 0xa54394fe1eedb8ff, 0x843610cb4bf160cc, 0xd389b47879823479, 0xa93af6c6c79b5d2e, 0x87625f056c7c4a8b, 0xd89d64d57a607745, 0xad4ab7112eb3929e, 0x8aa22c0dbef60ee4, 0xddd0467c64bce4a1, 0xb1736b96b6fd83b4, 0x8df5efabc5979c90, 0xe3231912d5bf60e6, 0xb5b5ada8aaff80b8, 0x915e2486ef32cd60, 0xe896a0d7e51e1566, 0xba121a4650e4ddec, 0x94db483840b717f0, 0xee2ba6c0678b597f, 0xbe89523386091466, 0x986ddb5c6b3a76b8, 0xf3e2f893dec3f126, 0xc31bfa0fe5698db8, 0x9c1661a651213e2d, 0xf9bd690a1b68637b, 0xc7caba6e7c5382c9, 0x9fd561f1fd0f9bd4, 0xffbbcfe994e5c620, 0xcc963fee10b7d1b3, 0xa3ab66580d5fdaf6, 0x82ef85133de648c5, 0xd17f3b51fca3a7a1, 0xa798fc4196e952e7, 0x8613fd0145877586, 0xd686619ba27255a3, 0xab9eb47c81f5114f, 0x894bc396ce5da772, 0xdbac6c247d62a584, 0xafbd2350644eead0, 0x8c974f7383725573, 0xe0f218b8d25088b8, 0xb3f4e093db73a093, 0x8ff71a0fe2c2e6dc, 0xe65829b3046b0afa, 0xb84687c269ef3bfb, 0x936b9fcebb25c996, 0xebdf661791d60f56, 0xbcb2b812db11a5de, 0x96f5600f15a7b7e5, 0xf18899b1bc3f8ca2, 0xc13a148e3032d6e8, 0x9a94dd3e8cf578ba, 0xf7549530e188c129, 0xc5dd44271ad3cdba, 0x9e4a9cec15763e2f, 0xfd442e4688bd304b, 0xca9cf1d206fdc03c, 0xa21727db38cb0030, 0x81ac1fe293d599c0, 0xcf79cc9db955c2cc, 0xa5fb0a17c777cf0a, 0x84c8d4dfd2c63f3b, 0xd47487cc8470652b, 0xa9f6d30a038d1dbc, 0x87f8a8d4cfa417ca, 0xd98ddaee19068c76, 0xae0b158b4738705f, 0x8b3c113c38f9f37f, 0xdec681f9f4c31f31, 0xb23867fb2a35b28e, 0x8e938662882af53e, 0xe41f3d6a7377eeca, 0xb67f6455292cbf08, 0x91ff83775423cc06, 0xe998d258869facd7, 0xbae0a846d2195713, 0x9580869f0e7aac0f, 0xef340a98172aace5, 0xbf5cd54678eef0b7, 0x991711052d8bf3c5, 0xf4f1b4d515acb93c, 0xc3f490aa77bd60fd, 0x9cc3a6eec6311a64, 0xfad2a4b13d1b5d6c, 0xc8a883c0fdaf7df0, 0xa086cfcd97bf97f4, 0x806bd9714632dff6, 0xcd795be870516656, 0xa46116538d0deb78, 0x8380dea93da4bc60, 0xd267caa862a12d67, 0xa8530886b54dbdec, 0x86a8d39ef77164bd, 0xd77485cb25823ac7, 0xac5d37d5b79b6239, 0x89e42caaf9491b61, 0xdca04777f541c568, 0xb080392cc4349ded, 0x8d3360f09cf6e4bd, 0xe1ebce4dc7f16dfc, 0xb4bca50b065abe63, 0x9096ea6f3848984f, 0xe757dd7ec07426e5, 0xb913179899f68584, 0x940f4613ae5ed137, 0xece53cec4a314ebe, 0xbd8430bd08277231, 0x979cf3ca6cec5b5b, 0xf294b943e17a2bc4, 0xc21094364dfb5637, 0x9b407691d7fc44f8, 0xf867241c8cc6d4c1, 0xc6b8e9b0709f109a, 0x9efa548d26e5a6e2, 0xfe5d54150b090b03, 0xcb7ddcdda26da269, 0xa2cb1717b52481ed, 0x823c12795db6ce57, 0xd0601d8efc57b08c, 0xa6b34ad8c9dfc070, 0x855c3be0a17fcd26, 0xd5605fcdcf32e1d7, 0xaab37fd7d8f58179, 0x888f99797a5e012d, 0xda7f5bf590966849, 0xaecc49914078536d, 0x8bd6a141006042be, 0xdfbdcece67006ac9, 0xb2fe3f0b8599ef08, 0x8f31cc0937ae58d3, 0xe51c79a85916f485, 0xb749faed14125d37, 0x92a1958a7675175f, 0xea9c227723ee8bcb, 0xbbb01b9283253ca3, 0x96267c7535b763b5, 0xf03d93eebc589f88, 0xc0314325637a193a, 0x99c102844f94e0fb, 0xf6019da07f549b2b, 0xc4ce17b399107c23, 0x9d71ac8fada6c9b5, 0xfbe9141915d7a922, 0xc987434744ac874f, 0xa139029f6a239f72, 0x80fa687f881c7f8e, 0xce5d73ff402d98e4, 0xa5178fff668ae0b6, 0x8412d9991ed58092, 0xd3515c2831559a83, 0xa90de3535aaae202, 0x873e4f75e2224e68, 0xd863b256369d4a41, 0xad1c8eab5ee43b67, 0x8a7d3eef7f1cfc52, 0xdd95317f31c7fa1d, 0xb1442798f49ffb4b, 0x8dd01fad907ffc3c, 0xe2e69915b3fff9f9, 0xb58547448ffffb2e, 0x91376c36d99995be, 0xe858ad248f5c22ca, 0xb9e08a83a5e34f08, 0x94b3a202eb1c3f39, 0xedec366b11c6cb8f, 0xbe5691ef416bd60c, 0x9845418c345644d7, 0xf3a20279ed56d48a, 0xc2e801fb244576d5, 0x9becce62836ac577, 0xf97ae3d0d2446f25, 0xc795830d75038c1e, 0x9faacf3df73609b1, 0xff77b1fcbebcdc4f, 0xcc5fc196fefd7d0c, 0xa37fce126597973d, 0x82cca4db847945ca, 0xd1476e2c07286faa, 0xa76c582338ed2622, 0x85f0468293f0eb4e, 0xd64d3d9db981787d, 0xab70fe17c79ac6ca, 0x892731ac9faf056f, 0xdb71e91432b1a24b, 0xaf8e5410288e1b6f, 0x8c71dcd9ba0b4926, 0xe0b62e2929aba83c, 0xb3c4f1ba87bc8697, 0x8fd0c16206306bac, 0xe61acf033d1a45df, 0xb8157268fdae9e4c, 0x93445b8731587ea3, 0xeba09271e88d976c, 0xbc807527ed3e12bd, 0x96cd2a865764dbca, 0xf148440a256e2c77, 0xc1069cd4eabe89f9, 0x9a6bb0aa55653b2d, 0xf712b443bbd52b7c, 0xc5a890362fddbc63, 0x9e20735e8cb16382, 0xfd00b897478238d1, 0xca66fa129f9b60a7, 0xa1ebfb4219491a1f, 0x818995ce7aa0e1b2, 0xcf42894a5dce35ea, 0xa5ced43b7e3e9188, 0x84a57695fe98746d, 0xd43bf0effdc0ba48, 0xa9c98d8ccb009506, 0x87d4713d6f33aa6c, 0xd953e8624b85dd79, 0xaddcb9e83c6b1794, 0x8b16fb203055ac76, 0xde8b2b66b3bc4724, 0xb208ef855c969f50, 0x8e6d8c6ab0787f73, 0xe3e27a444d8d98b8, 0xb64ec836a47146fa, 0x91d8a02bb6c10594, 0xe95a99df8ace6f54, 0xbaaee17fa23ebf76, 0x9558b4661b6565f8, 0xeef453d6923bd65a, 0xbf29dcaba82fdeae};
const double _10en[324 * 2 + 1] = {0, 1e-323, 1e-322, 1e-321, 1e-320, 1e-319, 1e-318, 1e-317, 1e-316, 1e-315, 1e-314, 1e-313, 1e-312, 1e-311, 1e-310, 1e-309, 1e-308, 1e-307, 1e-306, 1e-305, 1e-304, 1e-303, 1e-302, 1e-301, 1e-300, 1e-299, 1e-298, 1e-297, 1e-296, 1e-295, 1e-294, 1e-293, 1e-292, 1e-291, 1e-290, 1e-289, 1e-288, 1e-287, 1e-286, 1e-285, 1e-284, 1e-283, 1e-282, 1e-281, 1e-280, 1e-279, 1e-278, 1e-277, 1e-276, 1e-275, 1e-274, 1e-273, 1e-272, 1e-271, 1e-270, 1e-269, 1e-268, 1e-267, 1e-266, 1e-265, 1e-264, 1e-263, 1e-262, 1e-261, 1e-260, 1e-259, 1e-258, 1e-257, 1e-256, 1e-255, 1e-254, 1e-253, 1e-252, 1e-251, 1e-250, 1e-249, 1e-248, 1e-247, 1e-246, 1e-245, 1e-244, 1e-243, 1e-242, 1e-241, 1e-240, 1e-239, 1e-238, 1e-237, 1e-236, 1e-235, 1e-234, 1e-233, 1e-232, 1e-231, 1e-230, 1e-229, 1e-228, 1e-227, 1e-226, 1e-225, 1e-224, 1e-223, 1e-222, 1e-221, 1e-220, 1e-219, 1e-218, 1e-217, 1e-216, 1e-215, 1e-214, 1e-213, 1e-212, 1e-211, 1e-210, 1e-209, 1e-208, 1e-207, 1e-206, 1e-205, 1e-204, 1e-203, 1e-202, 1e-201, 1e-200, 1e-199, 1e-198, 1e-197, 1e-196, 1e-195, 1e-194, 1e-193, 1e-192, 1e-191, 1e-190, 1e-189, 1e-188, 1e-187, 1e-186, 1e-185, 1e-184, 1e-183, 1e-182, 1e-181, 1e-180, 1e-179, 1e-178, 1e-177, 1e-176, 1e-175, 1e-174, 1e-173, 1e-172, 1e-171, 1e-170, 1e-169, 1e-168, 1e-167, 1e-166, 1e-165, 1e-164, 1e-163, 1e-162, 1e-161, 1e-160, 1e-159, 1e-158, 1e-157, 1e-156, 1e-155, 1e-154, 1e-153, 1e-152, 1e-151, 1e-150, 1e-149, 1e-148, 1e-147, 1e-146, 1e-145, 1e-144, 1e-143, 1e-142, 1e-141, 1e-140, 1e-139, 1e-138, 1e-137, 1e-136, 1e-135, 1e-134, 1e-133, 1e-132, 1e-131, 1e-130, 1e-129, 1e-128, 1e-127, 1e-126, 1e-125, 1e-124, 1e-123, 1e-122, 1e-121, 1e-120, 1e-119, 1e-118, 1e-117, 1e-116, 1e-115, 1e-114, 1e-113, 1e-112, 1e-111, 1e-110, 1e-109, 1e-108, 1e-107, 1e-106, 1e-105, 1e-104, 1e-103, 1e-102, 1e-101, 1e-100, 1e-99, 1e-98, 1e-97, 1e-96, 1e-95, 1e-94, 1e-93, 1e-92, 1e-91, 1e-90, 1e-89, 1e-88, 1e-87, 1e-86, 1e-85, 1e-84, 1e-83, 1e-82, 1e-81, 1e-80, 1e-79, 1e-78, 1e-77, 1e-76, 1e-75, 1e-74, 1e-73, 1e-72, 1e-71, 1e-70, 1e-69, 1e-68, 1e-67, 1e-66, 1e-65, 1e-64, 1e-63, 1e-62, 1e-61, 1e-60, 1e-59, 1e-58, 1e-57, 1e-56, 1e-55, 1e-54, 1e-53, 1e-52, 1e-51, 1e-50, 1e-49, 1e-48, 1e-47, 1e-46, 1e-45, 1e-44, 1e-43, 1e-42, 1e-41, 1e-40, 1e-39, 1e-38, 1e-37, 1e-36, 1e-35, 1e-34, 1e-33, 1e-32, 1e-31, 1e-30, 1e-29, 1e-28, 1e-27, 1e-26, 1e-25, 1e-24, 1e-23, 1e-22, 1e-21, 1e-20, 1e-19, 1e-18, 1e-17, 1e-16, 1e-15, 1e-14, 1e-13, 1e-12, 1e-11, 1e-10, 1e-9, 1e-8, 1e-7, 1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21, 1e22, 1e23, 1e24, 1e25, 1e26, 1e27, 1e28, 1e29, 1e30, 1e31, 1e32, 1e33, 1e34, 1e35, 1e36, 1e37, 1e38, 1e39, 1e40, 1e41, 1e42, 1e43, 1e44, 1e45, 1e46, 1e47, 1e48, 1e49, 1e50, 1e51, 1e52, 1e53, 1e54, 1e55, 1e56, 1e57, 1e58, 1e59, 1e60, 1e61, 1e62, 1e63, 1e64, 1e65, 1e66, 1e67, 1e68, 1e69, 1e70, 1e71, 1e72, 1e73, 1e74, 1e75, 1e76, 1e77, 1e78, 1e79, 1e80, 1e81, 1e82, 1e83, 1e84, 1e85, 1e86, 1e87, 1e88, 1e89, 1e90, 1e91, 1e92, 1e93, 1e94, 1e95, 1e96, 1e97, 1e98, 1e99, 1e100, 1e101, 1e102, 1e103, 1e104, 1e105, 1e106, 1e107, 1e108, 1e109, 1e110, 1e111, 1e112, 1e113, 1e114, 1e115, 1e116, 1e117, 1e118, 1e119, 1e120, 1e121, 1e122, 1e123, 1e124, 1e125, 1e126, 1e127, 1e128, 1e129, 1e130, 1e131, 1e132, 1e133, 1e134, 1e135, 1e136, 1e137, 1e138, 1e139, 1e140, 1e141, 1e142, 1e143, 1e144, 1e145, 1e146, 1e147, 1e148, 1e149, 1e150, 1e151, 1e152, 1e153, 1e154, 1e155, 1e156, 1e157, 1e158, 1e159, 1e160, 1e161, 1e162, 1e163, 1e164, 1e165, 1e166, 1e167, 1e168, 1e169, 1e170, 1e171, 1e172, 1e173, 1e174, 1e175, 1e176, 1e177, 1e178, 1e179, 1e180, 1e181, 1e182, 1e183, 1e184, 1e185, 1e186, 1e187, 1e188, 1e189, 1e190, 1e191, 1e192, 1e193, 1e194, 1e195, 1e196, 1e197, 1e198, 1e199, 1e200, 1e201, 1e202, 1e203, 1e204, 1e205, 1e206, 1e207, 1e208, 1e209, 1e210, 1e211, 1e212, 1e213, 1e214, 1e215, 1e216, 1e217, 1e218, 1e219, 1e220, 1e221, 1e222, 1e223, 1e224, 1e225, 1e226, 1e227, 1e228, 1e229, 1e230, 1e231, 1e232, 1e233, 1e234, 1e235, 1e236, 1e237, 1e238, 1e239, 1e240, 1e241, 1e242, 1e243, 1e244, 1e245, 1e246, 1e247, 1e248, 1e249, 1e250, 1e251, 1e252, 1e253, 1e254, 1e255, 1e256, 1e257, 1e258, 1e259, 1e260, 1e261, 1e262, 1e263, 1e264, 1e265, 1e266, 1e267, 1e268, 1e269, 1e270, 1e271, 1e272, 1e273, 1e274, 1e275, 1e276, 1e277, 1e278, 1e279, 1e280, 1e281, 1e282, 1e283, 1e284, 1e285, 1e286, 1e287, 1e288, 1e289, 1e290, 1e291, 1e292, 1e293, 1e294, 1e295, 1e296, 1e297, 1e298, 1e299, 1e300, 1e301, 1e302, 1e303, 1e304, 1e305, 1e306, 1e307, 1e308, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16};

static inline ascii16 Convert16Digits(uint64_t abcdefgh, uint64_t ijklmnop)
{

#if HAS_SSE2

#if defined(__AVX512IFMA__) && defined(__AVX512VBMI__) //&& (false)
    const __m512i bcstq_h = _mm512_set1_epi64(abcdefgh);
    const __m512i bcstq_l = _mm512_set1_epi64(ijklmnop);
    const __m512i zmmzero = _mm512_castsi128_si512(_mm_cvtsi64_si128(0x1A1A400));
    const __m512i zmmTen = _mm512_set1_epi64(10);
    const __m512i zero = _mm512_set1_epi64(0);
    const __m512i ifma_const = _mm512_setr_epi64(0x00000000002af31dc, 0x0000000001ad7f29b, 0x0000000010c6f7a0c, 0x00000000a7c5ac472,
                                                 0x000000068db8bac72, 0x0000004189374bc6b, 0x0000028f5c28f5c29, 0x0000199999999999a);
#if 0
    const __m512i permb_const = _mm512_castsi128_si512(_mm_set_epi8(0x78, 0x70, 0x68, 0x60, 0x58, 0x50, 0x48, 0x40, 0x38, 0x30, 0x28, 0x20, 0x18, 0x10, 0x08, 0x00));
    __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_h = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_h);
    __m512i highbits_l = _mm512_madd52hi_epu64(zero, zmmTen, lowbits_l);
    __m512i bcd = _mm512_permutex2var_epi8(highbits_h, permb_const, highbits_l);
#else
    const __m512i permb_const7 = _mm512_castsi128_si512(_mm_set_epi8(0x7f, 0x77, 0x6f, 0x67, 0x5f, 0x57, 0x4f, 0x47, 0x3f, 0x37, 0x2f, 0x27, 0x1f, 0x17, 0x0f, 0x07));
    __m512i lowbits_h = _mm512_madd52lo_epu64(zmmzero, bcstq_h, ifma_const);
    __m512i lowbits_l = _mm512_madd52lo_epu64(zmmzero, bcstq_l, ifma_const);
    __m512i highbits_h7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_h, 5), _mm512_slli_epi64(lowbits_h, 7));
    __m512i highbits_l7 = _mm512_add_epi64(_mm512_slli_epi64(lowbits_l, 5), _mm512_slli_epi64(lowbits_l, 7));
    __m512i bcd = _mm512_permutex2var_epi8(highbits_h7, permb_const7, highbits_l7); // another way to permute , but slower.
#endif
    __m128i little_endian_bcd = _mm512_castsi512_si128(bcd);
    __m128i little_endian_ascii = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
    return {little_endian_ascii};
#else // sse2
    __m128i x = _mm_unpacklo_epi64(_mm_cvtsi32_si128(abcdefgh), _mm_cvtsi32_si128(ijklmnop));
    //__m128i x = _mm_set_epi64x(ijklmnop, abcdefgh);
    __m128i y = _mm_add_epi64(x, _mm_mul_epu32(_mm_set1_epi64x((1ull << 32) - 10000), _mm_srli_epi64(_mm_mul_epu32(x, _mm_set1_epi64x(109951163)), 40)));
#if defined(__SSE4_1__)
    __m128i z = _mm_sub_epi32(_mm_slli_epi32(y, 16), _mm_mullo_epi32(_mm_set1_epi32((100 << 16) - 1), _mm_srli_epi32(_mm_mulhi_epi16(y, _mm_set1_epi32(10486)), 4)));
#else
    __m128i y_div_100 = _mm_srli_epi16(_mm_mulhi_epu16(y, _mm_set1_epi16(0x147b)), 3);
    __m128i y_mod_100 = _mm_sub_epi16(y, _mm_mullo_epi16(y_div_100, _mm_set1_epi16(100)));
    __m128i z = _mm_or_si128(y_div_100, _mm_slli_epi32(y_mod_100, 16));
#endif
    __m128i z_div_10 = _mm_mulhi_epu16(z, _mm_set1_epi16(0x199a));
    __m128i bcd_swapped = _mm_sub_epi16(_mm_slli_epi16(z, 8), _mm_mullo_epi16(_mm_set1_epi16(2559), z_div_10));
    __m128i little_endian_bcd = _mm_shuffle_epi32(bcd_swapped, _MM_SHUFFLE(2, 3, 0, 1));
    __m128i little_endian_ascii = _mm_add_epi8(little_endian_bcd, _mm_set1_epi8('0'));
    return {little_endian_ascii};
#endif

#endif

#if !HAS_NEON_OR_SSE2
    uint64_t abcd_efgh = abcdefgh + (0x100000000 - 10000) * ((abcdefgh * 0x68db8bbull) >> 40);
    uint64_t ijkl_mnop = ijklmnop + (0x100000000 - 10000) * ((ijklmnop * 0x68db8bbull) >> 40);
    uint64_t ab_cd_ef_gh = abcd_efgh + (0x10000 - 100) * (((abcd_efgh * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t ij_kl_mn_op = ijkl_mnop + (0x10000 - 100) * (((ijkl_mnop * 0x147b) >> 19) & 0x7f0000007f);
    uint64_t a_b_c_d_e_f_g_h = ab_cd_ef_gh + (0x100 - 10) * (((ab_cd_ef_gh * 0x67) >> 10) & 0xf000f000f000f);
    uint64_t i_j_k_l_m_n_o_p = ij_kl_mn_op + (0x100 - 10) * (((ij_kl_mn_op * 0x67) >> 10) & 0xf000f000f000f);
    //int abcdefgh_tz = u64_tz_bits(a_b_c_d_e_f_g_h);
    //int ijklmnop_tz = u64_tz_bits(i_j_k_l_m_n_o_p);
    uint64_t abcdefgh_bcd = is_little_endian() ? byteswap64(a_b_c_d_e_f_g_h) : a_b_c_d_e_f_g_h;
    uint64_t ijklmnop_bcd = is_little_endian() ? byteswap64(i_j_k_l_m_n_o_p) : i_j_k_l_m_n_o_p;
    //int tz = (ijklmnop == 0) ? 64 + abcdefgh_tz : ijklmnop_tz;
    //tz = tz / 8;
    return {abcdefgh_bcd | ZERO, ijklmnop_bcd | ZERO};
#endif

}
struct exp_result_table
{
    uint64_t data[308 - (-324) + 1] = {}; // [-324 , 308]

    constexpr u64 operator[](int e10) const
    {
        return data[e10 + 324];
    }

    constexpr exp_result_table()
    {
        for (int e10 = -324; e10 <= 308; e10++)
        {
            u64 e = e10 < 0 ? ('e' + '-' * 256) : 'e' + '+' * 256;
            u64 e10_abs = e10 < 0 ? -e10 : e10;
            u64 a = e10_abs / 100;
            u64 bc = e10_abs - a * 100;
            u64 b = bc / 10;
            u64 c = bc - b * 10;
            u64 exp_len = 4 + (e10_abs >= 100);
            u64 e10_abs_ascii = (e10_abs >= 100) ? (a + '0') + ((b + '0') << 8) + ((c + '0') << 16) : (b + '0') + ((c + '0') << 8);
            u64 exp_res = e + (e10_abs_ascii << 16) + (exp_len << 56);
            data[e10 + 324] = exp_res;
        }
    }
};
alignas(64) constexpr exp_result_table exp_result;
extern "C" char *d2e_impl(double v, char *buffer)
{
    const int Precision = 16;
    buffer[0] = '-';
    u64 vi;
    memcpy(&vi, &v, sizeof(u64));
    buffer += vi >> 63;
    u64 vi_abs = vi & (((u64)1 << 63) - 1);
    i64 ieee754_exp11 = (vi_abs >> 52);
    if (ieee754_exp11 == 0x7ff) // nan or inf
        return (char *)memcpy(buffer, (vi_abs == ((u64)0x7ff << 52)) ? "inf" : "nan", 4) + 3;
    u64 f = (vi << 11) | (((u64)1) << 63);
    i64 e2 = ieee754_exp11 - 1023;//max = 2046-1023=1023
    if (ieee754_exp11 == 0) [[unlikely]]
    {
        if (vi_abs == 0)
            return (char *)memcpy(buffer, "0", 2) + 1;
        u64 clz = u64_lz_bits(vi_abs);//clz max = 63;
        f = vi_abs << clz;
        e2 = -1011 - clz;//min = -1074
    }
    const u64 *f64_pow10_ptr = (u64 *)&_10en[1 + 324];
    const u64 *power_ptr = (u64 *)&powers_ten_reverse[343 - Precision];
    const u64 *exp_ptr = (u64 *)&exp_result.data[324];
    i64 e10_tmp = (e2 * 78913) >> 18;                     // == floor(e2*log10(2))
    i64 e10 = e10_tmp + (vi_abs >= f64_pow10_ptr[e10_tmp]); // e10_tmp or e10_tmp+1
    u64 pow10_f = power_ptr[e10];//e10 : [-324]
    //i64 shift = 61 - e2 - (((Precision - e10) * 217707) >> 16);
    i64 shift = 61 - (e2 + (((Precision - e10) * 217707) >> 16));
    u64 m = ((u128)(f)*pow10_f) >> 64;
    u64 m2 = (m >> shift) + 1;
    u64 h9 = m2 / (u64)2e8;
    u64 h1 = m2 / (u64)2e16;
    u64 first_digit = h1 | ('0' + ((int)'.' << 8));
    memcpy(buffer, &first_digit, 2);
    u64 abcdefgh = h9 + h1 * (i64)-1e8;
    u64 ijklmnop = (m2 >> 1) + h9 * (i64)-1e8;
    ascii16 ascii = Convert16Digits(abcdefgh, ijklmnop);
    memcpy(&buffer[2], &ascii, 16);
    buffer += 18;
    memcpy(buffer, &exp_ptr[e10], 8);
    return buffer + (exp_ptr[e10] >> 56);
}