#include "/inc/base.inc"
#include "/inc/testarray.inc"
#include "/inc/gc.inc"
#include "/inc/deep_eq.inc"

mixed *tests = ({
    ({ "Unprotected local 1", 0,  (: int a; a = 10; return a == 10; :) }),
    ({ "Unprotected local 2", 0,  (: int a; a += 10; return a == 10; :) }),
    ({ "Unprotected char 1", 0, (: string str="xyz"; str[2]='x'; return str=="xyx"; :) }),
    ({ "Unprotected range 1", 0, (: mixed a = ({1})*10; a[2..6]   = ({2})*5; return deep_eq(a, ({1})*2+({2})*5+({1})*3); :) }),
    ({ "Unprotected range 2", 0, (: mixed a = ({1})*10; a[2..6] &&= ({2})*5; return deep_eq(a, ({1})*2+({2})*5+({1})*3); :) }),
    ({ "Empty mapping entries (#148)", 0, (: mapping m=([:1]); catch(m[1][2]=3); return !sizeof(m); :) }),
    /* TODO: What should the proper results be? */
    ({ "Vanishing destinations 1", TF_ERROR, (: mixed a = ({1}); a[0] = (a=0); :) }),
    ({ "Vanishing destinations 2", TF_ERROR, (: mixed a = ([]); a["abc"] = (a=0); :) }),
    ({ "Vanishing destinations 3", TF_ERROR, (: string str = "abcd"; str[0] = (str=0); :) }),
    ({ "Vanishing destinations 4", TF_DONTCHECKERROR, (: mixed a = ({1}); a[0] &&= (a=0); :) }),
    /* If the previous one didn't crash, let's try to detect its wrongdoing */
    ({ "Vanishing destinations 4b", 0,
        (:
            mixed a = ({1}),b;

            a[0] &&= ((a=0), (b=({2})), 0);
            return b[0]==2;
        :)
    }),
    ({ "Vanishing destinations 5", TF_DONTCHECKERROR, (: mixed a = ([]); a["abc"] ||= (a=0); :) }),
    ({ "Vanishing destinations 6", TF_DONTCHECKERROR, (: string str = "abcd"; str[0] &&= (str=0); :) }),
    ({ "Vanishing destinations 7", TF_DONTCHECKERROR, (: mixed a = ({1})*10; a[2..6] &&= ((a=0), ({2})*10); :) }),
    ({ "Protected char 1", 0, (: string str = "xyz"; mixed c = 0 || &(str[1]); c = 'a'; return str == "xaz"; :) }),
    ({ "Protected char 2", 0, (: string str = "xyz"; mixed c1 = 0 || &(str[0]), c2 = 0 || &(str[1]); c1 = 'a'; c2 = 'b';return str == "abz"; :) }),
    ({ "Protected char 3", 0,
        (:
            string str1,str2;
            mixed c;

            str1="abcdefgh"; 
            c = (0||&(str1[3]));
            c='1';

            str2=str1;
            c='2'; /* Should only change str1. */
            return str2 != str1;
         :)
    }),
    ({ "Protected char 4", 0,
        (:
            mapping m = ([:1]);
            string str;
            mixed c;
            string *ind;

            str = "abcdefhi";
            c = (0||&(str[3]));
            c = '1';
            m[str] = "Nr. 1";
            c = '2';
            m["abc2efghi"] = "Nr. 2";

            ind = m_indices(m);
            return sizeof(m)==2 && m[1] != m[2];
        :)
    }),
    ({ "Volatile protected lvalues 1", 0,
        (:
            mixed val = 10;
            mixed lv1 = 0 || &val;
            mixed lv2 = 1;

            lv2 = lv1; /* Only the value (10) should be copied. */
            lv2 = 20;

            return val == 10; /* Shouldn't have changed. */
        :)
    }),
    ({ "Volatile protected lvalues 2", 0,
        (:
            mixed val = 10;
            mixed arr1 = ({ 0 || &val});
            mixed arr2 = ({ 1 });

            arr2[0] = arr1[0]; /* Only the value (10) should be copied. */
            arr2[0] = 20;

            return val == 10; /* Shouldn't have changed. */
        :)
    }),
    ({ "Reference loops", 0,
       (:
           mixed a = 10;
           mixed b =  &a;
           a = &b;

           return (a==10);
       :)
    })
});

void run_test()
{
    msg("\nRunning test suite for lvalues:\n"
          "-------------------------------\n");

    run_array(tests,
        (:
            if($1)
                shutdown(1);
            else
                start_gc(#'shutdown);

            return 0;
        :));
}

string *epilog(int eflag)
{
    run_test();
    return 0;
}

int f(int arg)
{
    return arg + 1;
}
