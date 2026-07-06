/* Make sure, this struct definition doesn't leak memory.
 */
#pragma save_local_names

struct TestStruct {};

void fun_using_struct(struct TestStruct value) {}

// Not for compile_string()
int run_test()
{
    destruct(this_object());
    return 1;
}
