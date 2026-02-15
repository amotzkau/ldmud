// Check that if we abort a charmode mid-buffer, the driver
// will process the rest normally.

#include "/inc/base.inc"
#include "/inc/client.inc"

#include "/sys/commands.h"
#include "/sys/input_to.h"

void run_client()
{
   write("Hello!");
   call_out(#'write, __ALARM_TIME__, "\n");
}

void run_server()
{
    configure_interactive(this_object(), IC_TELNET_ENABLED, 1);

    input_to("client_char_input", INPUT_CHARMODE);
    add_action("client_line_input", "ello", AA_SHORT);
}

void client_char_input(string str)
{
    if (str != "H")
    {
        msg("FAILURE: Didn't receive a single char, got %Q instead.\n", str);
        shutdown(1);
        return;
    }
}

void client_line_input(string str)
{
    if (query_verb() != "ello!")
    {
        msg("FAILURE: Didn't receive correct rest of the input, got %Q instead.\n", query_verb());
        shutdown(1);
        return;
    }

    msg("Success.\n");
    shutdown(0);
}

void abort()
{
    msg("FAILURE: Didn't receive input.\n");
    shutdown(1);
}

void run_test()
{
    msg("\nRunning test for stopping charmode #2:\n"
          "--------------------------------------\n");

    connect_self("run_server", "run_client");

    call_out(#'abort, 2*__ALARM_TIME__);
}

string *epilog(int eflag)
{
    run_test();
    return 0;
}
