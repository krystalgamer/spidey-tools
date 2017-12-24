/* 
 * Purpose: forwards the calls to the original dll
 */

// Took from SO
// https://stackoverflow.com/questions/39688983/mingw-forward-dll-call
// Apparently it is realted to a tool called 'dlltool' which is not always present according to the gnu docs

#ifdef _MSC_VER
    #define FORWARDED_EXPORT_WITH_ORDINAL(exp_name, ordinal, target_name) __pragma (comment (linker, "/export:" #exp_name "=" #target_name ",@" #ordinal))
#endif
#ifdef __GNUC__
    #define FORWARDED_EXPORT_WITH_ORDINAL(exp_name, ordinal, target_name) asm (".section .drectve\n\t.ascii \" -export:" #exp_name "= " #target_name " @" #ordinal "\"");
#endif


