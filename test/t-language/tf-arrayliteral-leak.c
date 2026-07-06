/* The argument types from an incomplete array literal should not leak.
 * We're using a struct here to make the type unique across the test suite.
 */

struct X {};

mixed x = ({ ({ (<X>) })
