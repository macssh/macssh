TS_MESSAGE(»lsh_get_cstring 1«)
{
	struct lsh_string *s = TS_STRING("foo");
	const char *p = lsh_get_cstring(s);
	TS_CHECK(p && !strcmp(p, "foo")) 
}
TS_MESSAGE(»lsh_get_cstring 2«)
{
	struct lsh_string *s = TS_STRING(#66006f#);
	TS_CHECK(!lsh_get_cstring(s))
}
TS_MESSAGE(»lsh_get_cstring 3«)
{
	struct lsh_string *s = TS_STRING(#6600#);
	TS_CHECK(!lsh_get_cstring(s))
}
TS_MESSAGE(»lsh_get_cstring 3«)
{
	struct lsh_string *s = TS_STRING(##);
	const char *p = lsh_get_cstring(s);
	TS_CHECK(p && !*p)
}
