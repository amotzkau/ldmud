#include "/inc/base.inc"
#include "/inc/testarray.inc"
#include "/inc/gc.inc"
#include "/inc/deep_eq.inc"

#pragma no_warn_missing_return

mixed *tests = ({
    ({ "Unprotected local 1", 0, (: int a; a = 10; return a == 10; :) }),
    ({ "Unprotected local 2", 0, (: int a; a += 10; return a == 10; :) }),
    ({ "Unprotected local 3", 0, (: mixed a = ({1,2,3}); a *= 2; return deep_eq(a, ({1,2,3,1,2,3})); :) }),
    ({ "Unprotected char 1", 0,  (: string str="xyz"; str[2]='x'; return str=="xyx"; :) }),
    ({ "Unprotected char 2", 0,  (: string str="@@@"; str[1] /= 2; return str == "@ @"; :) }),
    ({ "Unprotected char 3", 0,  (: string str="@@@"; str[1] &&= 'A'; return str == "@A@"; :) }),
    ({ "Unprotected pointer range 1", 0, (: mixed a = ({1})*10; a[2..6]   = ({2})*5; return deep_eq(a, ({1})*2+({2})*5+({1})*3); :) }),
    ({ "Unprotected pointer range 2", 0, (: mixed a = ({1})*10; a[2..6] &&= ({2})*5; return deep_eq(a, ({1})*2+({2})*5+({1})*3); :) }),
    ({ "Unprotected pointer range 3", 0, (: mixed a = ({1})*10; a[2..6]  *= 2; return deep_eq(a, ({1})*15); :) }),
    ({ "Unprotected string range 1", 0, (: string a = "0123456789"; a[2..6]   = "xxxxx"; return a == "01xxxxx789"; :) }),
    ({ "Unprotected string range 2", 0, (: string a = "0123456789"; a[2..6] &&= "xxxxx"; return a == "01xxxxx789"; :) }),
    ({ "Unprotected string range 3", 0, (: string a = "0123456789"; a[2..6]  *= 2; return a == "012345623456789"; :) }),
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
    ({ "Protected locals 1", 0, (: mixed a = ({1,2,3,4,5}); int summe; map(a, (: $2+=$1 :), &summe); return summe == 15; :) }),
    ({ "Protected locals 2", 0, (: int x = 1; return &x; :) }),
    ({ "Protected elements 1", 0, (: mixed a = ({1,2,3,4,5}); funcall((: $1 = $2; $2 = $3; :), &(a[1]), &(a[2]), &(a[<2])); return deep_eq(a, ({1,3,4,4,5})); :) }),
    ({ "Protected elements 2", 0, (: mixed a = ({1,2,3,4,5}); funcall((: $1 = $2; $2 = $3; :), &(a[1]), &(a[2]), &(a[1])); return deep_eq(a, ({1,3,3,4,5})); :) }),
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
    ({ "Protected pointer range 1", 0, (: mixed a = ({1,2,3,4,5}); funcall((: $1 = $2; :), &(a[1..2]), &(a[3..3])); return deep_eq(a, ({1,4,4,5})); :) }),
    ({ "Protected pointer range 2", 0, (: mixed a = ({1,2,3,4,5}); funcall((: $1[1] = "a"; :), &(a[1..2])); return deep_eq(a, ({1,2,"a",4,5})); :) }),
    // Some challenges:
    ({ "Protected pointer range 3", 0, (: mixed a = ({1,2,3,4,5}); funcall((: $1 = $3; $2 = $3; :), &(a[1..2]), &(a[3..3]), &(a[4..4])); return deep_eq(a, ({1,5,5,5})); :) }),
    ({ "Protected pointer range 4", 0,
       (:
           mixed a = ({1,2,3,4,5}), b = ({6,7,8,9,0});
           mixed x = &(a[1..2]), y = &(b[1..2]);

           x = &y;

           y = ({10, 11, 12});

           return deep_eq(a, ({1,10,11,12,4,5})) && deep_eq(b, ({6,10,11,12,9,0}));
       :)
    }),
    ({ "Protected string range 1", 0, (: string a = "12345"; funcall((: $1 = $2; :), &(a[1..2]), &(a[3..3])); return a == "1445"; :) }),
    ({ "Protected string range 2", 0, (: string a = "12345"; funcall((: $1[1] = 'a'; :), &(a[1..2])); return a == "12a45"; :) }),
    ({ "Protected string range 3", 0, (: string a = "12345"; funcall((: $1 = $3; $2 = $3; :), &(a[1..2]), &(a[3..3]), &(a[4..4])); return a == "1555"; :) }),
    ({ "Protected string range 4", 0,
       (:
           string a = "12345", b = "67890";
           mixed x = &(a[1..2]), y = &(b[1..2]);

           x = &y;

           y = "abc";

           return a == "1abc45" && b == "6abc90";
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
    }),
    ({ "Special Efuns 1", 0,
       (:
           mixed a = ({ 1 });
           int nr;
           nr = sscanf("5 6","%d %d", a[0], a);
           return nr == 2 && a == 5;
       :)
    }),
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
