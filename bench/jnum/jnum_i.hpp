namespace jnum {
    #include "jnum.c"
}
char* jnum_f64_to_str(double num, char *buffer){
    int len = jnum::jnum_dtoa(num, buffer);
    //int len = 0;
    return buffer + len;
}
char* jnum_f32_to_str(float num, char *buffer){
    int len = jnum::jnum_dtoa(num, buffer);
    //int len = 0;
    return buffer + len;
}