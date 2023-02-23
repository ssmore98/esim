CC = g++ -std=c++11
CFLAGS = -Wall -Werror -ggdb3 # -O2
C_FILES = sim.cpp server.cpp event.cpp generator.cpp task.cpp
O_FILES = $(C_FILES:.cpp=.o)
DEFAULT_INCLUDES =  -I/usr/include/c++/9 -I/usr/include/x86_64-linux-gnu/c++/9 -I/usr/include/c++/9/backward \
		    -I/usr/lib/gcc/x86_64-linux-gnu/9/include -I/usr/local/include -I/usr/include/x86_64-linux-gnu -I/usr/include

default: esim

%.o: %.cpp Makefile
	${CC} ${CFLAGS} -c $<

esim: $(O_FILES)
	$(CC) $(O_FILES) -ggdb3 -o $@ -lyaml 

depend: Makefile $(C_FILES)
	makedepend $(DEFAULT_INCLUDES) $(C_FILES)
	sudo apt install `find-dbgsym-packages esim` -y

clean:
	\rm -f $(O_FILES)
# DO NOT DELETE

sim.o: /usr/include/c++/9/map /usr/include/c++/9/bits/stl_tree.h
sim.o: /usr/include/c++/9/bits/stl_algobase.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++config.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/os_defines.h
sim.o: /usr/include/features.h /usr/include/stdc-predef.h
sim.o: /usr/include/x86_64-linux-gnu/sys/cdefs.h
sim.o: /usr/include/x86_64-linux-gnu/bits/wordsize.h
sim.o: /usr/include/x86_64-linux-gnu/bits/long-double.h
sim.o: /usr/include/x86_64-linux-gnu/gnu/stubs.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h
sim.o: /usr/include/c++/9/bits/functexcept.h
sim.o: /usr/include/c++/9/bits/exception_defines.h
sim.o: /usr/include/c++/9/bits/cpp_type_traits.h
sim.o: /usr/include/c++/9/ext/type_traits.h
sim.o: /usr/include/c++/9/ext/numeric_traits.h
sim.o: /usr/include/c++/9/bits/stl_pair.h /usr/include/c++/9/bits/move.h
sim.o: /usr/include/c++/9/bits/concept_check.h
sim.o: /usr/include/c++/9/bits/stl_iterator_base_types.h
sim.o: /usr/include/c++/9/bits/stl_iterator_base_funcs.h
sim.o: /usr/include/c++/9/debug/assertions.h
sim.o: /usr/include/c++/9/bits/stl_iterator.h
sim.o: /usr/include/c++/9/bits/ptr_traits.h /usr/include/c++/9/debug/debug.h
sim.o: /usr/include/c++/9/bits/predefined_ops.h
sim.o: /usr/include/c++/9/bits/allocator.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++allocator.h
sim.o: /usr/include/c++/9/ext/new_allocator.h /usr/include/c++/9/new
sim.o: /usr/include/c++/9/exception /usr/include/c++/9/bits/exception.h
sim.o: /usr/include/c++/9/bits/memoryfwd.h
sim.o: /usr/include/c++/9/bits/stl_function.h
sim.o: /usr/include/c++/9/backward/binders.h
sim.o: /usr/include/c++/9/ext/alloc_traits.h
sim.o: /usr/include/c++/9/bits/stl_map.h
sim.o: /usr/include/c++/9/bits/stl_multimap.h
sim.o: /usr/include/c++/9/bits/range_access.h
sim.o: /usr/include/c++/9/bits/erase_if.h /usr/include/yaml.h
sim.o: /usr/include/c++/9/stdlib.h /usr/include/stdlib.h
sim.o: /usr/include/x86_64-linux-gnu/bits/libc-header-start.h
sim.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h
sim.o: /usr/include/x86_64-linux-gnu/bits/waitflags.h
sim.o: /usr/include/x86_64-linux-gnu/bits/waitstatus.h
sim.o: /usr/include/x86_64-linux-gnu/bits/floatn.h
sim.o: /usr/include/x86_64-linux-gnu/bits/floatn-common.h
sim.o: /usr/include/x86_64-linux-gnu/sys/types.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types.h
sim.o: /usr/include/x86_64-linux-gnu/bits/timesize.h
sim.o: /usr/include/x86_64-linux-gnu/bits/typesizes.h
sim.o: /usr/include/x86_64-linux-gnu/bits/time64.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/clock_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/time_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/timer_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/stdint-intn.h /usr/include/endian.h
sim.o: /usr/include/x86_64-linux-gnu/bits/endian.h
sim.o: /usr/include/x86_64-linux-gnu/bits/endianness.h
sim.o: /usr/include/x86_64-linux-gnu/bits/byteswap.h
sim.o: /usr/include/x86_64-linux-gnu/bits/uintn-identity.h
sim.o: /usr/include/x86_64-linux-gnu/sys/select.h
sim.o: /usr/include/x86_64-linux-gnu/bits/select.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h
sim.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h
sim.o: /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h
sim.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h
sim.o: /usr/include/x86_64-linux-gnu/bits/struct_mutex.h
sim.o: /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h
sim.o: /usr/include/alloca.h
sim.o: /usr/include/x86_64-linux-gnu/bits/stdlib-float.h /usr/include/stdio.h
sim.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h
sim.o: /usr/include/x86_64-linux-gnu/bits/stdio_lim.h
sim.o: /usr/include/x86_64-linux-gnu/bits/sys_errlist.h /usr/include/string.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/locale_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h
sim.o: /usr/include/strings.h server.h
sim.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h
sim.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint-gcc.h
sim.o: /usr/include/c++/9/random /usr/include/c++/9/bits/c++0x_warning.h
sim.o: task.h /usr/include/c++/9/deque
sim.o: /usr/include/c++/9/bits/stl_construct.h
sim.o: /usr/include/c++/9/bits/stl_uninitialized.h
sim.o: /usr/include/c++/9/bits/stl_deque.h /usr/include/c++/9/bits/deque.tcc
sim.o: /usr/include/c++/9/iostream /usr/include/c++/9/ostream
sim.o: /usr/include/c++/9/ios /usr/include/c++/9/iosfwd
sim.o: /usr/include/c++/9/bits/stringfwd.h /usr/include/c++/9/bits/postypes.h
sim.o: /usr/include/c++/9/cwchar /usr/include/wchar.h
sim.o: /usr/include/x86_64-linux-gnu/bits/wchar.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/wint_t.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h
sim.o: /usr/include/c++/9/bits/char_traits.h
sim.o: /usr/include/c++/9/bits/localefwd.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++locale.h
sim.o: /usr/include/c++/9/clocale /usr/include/locale.h
sim.o: /usr/include/x86_64-linux-gnu/bits/locale.h /usr/include/c++/9/cctype
sim.o: /usr/include/ctype.h /usr/include/c++/9/bits/ios_base.h
sim.o: /usr/include/c++/9/ext/atomicity.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h
sim.o: /usr/include/pthread.h /usr/include/sched.h
sim.o: /usr/include/x86_64-linux-gnu/bits/sched.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h
sim.o: /usr/include/x86_64-linux-gnu/bits/cpu-set.h /usr/include/time.h
sim.o: /usr/include/x86_64-linux-gnu/bits/time.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h
sim.o: /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h
sim.o: /usr/include/x86_64-linux-gnu/bits/setjmp.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/atomic_word.h
sim.o: /usr/include/c++/9/bits/locale_classes.h /usr/include/c++/9/string
sim.o: /usr/include/c++/9/bits/ostream_insert.h
sim.o: /usr/include/c++/9/bits/cxxabi_forced.h
sim.o: /usr/include/c++/9/bits/basic_string.h
sim.o: /usr/include/c++/9/bits/basic_string.tcc
sim.o: /usr/include/c++/9/bits/locale_classes.tcc
sim.o: /usr/include/c++/9/stdexcept /usr/include/c++/9/streambuf
sim.o: /usr/include/c++/9/bits/streambuf.tcc
sim.o: /usr/include/c++/9/bits/basic_ios.h
sim.o: /usr/include/c++/9/bits/locale_facets.h /usr/include/c++/9/cwctype
sim.o: /usr/include/wctype.h
sim.o: /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_base.h
sim.o: /usr/include/c++/9/bits/streambuf_iterator.h
sim.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_inline.h
sim.o: /usr/include/c++/9/bits/locale_facets.tcc
sim.o: /usr/include/c++/9/bits/basic_ios.tcc
sim.o: /usr/include/c++/9/bits/ostream.tcc /usr/include/c++/9/istream
sim.o: /usr/include/c++/9/bits/istream.tcc /usr/include/c++/9/cassert
sim.o: /usr/include/assert.h /usr/include/c++/9/vector
sim.o: /usr/include/c++/9/bits/stl_vector.h
sim.o: /usr/include/c++/9/bits/stl_bvector.h
sim.o: /usr/include/c++/9/bits/vector.tcc event.h
sim.o: /usr/include/c++/9/algorithm /usr/include/c++/9/utility
sim.o: /usr/include/c++/9/bits/stl_relops.h
sim.o: /usr/include/c++/9/bits/stl_algo.h /usr/include/c++/9/cstdlib
sim.o: /usr/include/stdlib.h /usr/include/c++/9/bits/std_abs.h
sim.o: /usr/include/stdlib.h /usr/include/c++/9/bits/algorithmfwd.h
sim.o: /usr/include/c++/9/bits/stl_heap.h
sim.o: /usr/include/c++/9/bits/stl_tempbuf.h generator.h
server.o: /usr/include/c++/9/chrono /usr/include/c++/9/bits/c++0x_warning.h
server.o: /usr/include/c++/9/iostream
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++config.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/os_defines.h
server.o: /usr/include/features.h /usr/include/stdc-predef.h
server.o: /usr/include/x86_64-linux-gnu/sys/cdefs.h
server.o: /usr/include/x86_64-linux-gnu/bits/wordsize.h
server.o: /usr/include/x86_64-linux-gnu/bits/long-double.h
server.o: /usr/include/x86_64-linux-gnu/gnu/stubs.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h
server.o: /usr/include/c++/9/ostream /usr/include/c++/9/ios
server.o: /usr/include/c++/9/iosfwd /usr/include/c++/9/bits/stringfwd.h
server.o: /usr/include/c++/9/bits/memoryfwd.h
server.o: /usr/include/c++/9/bits/postypes.h /usr/include/c++/9/cwchar
server.o: /usr/include/wchar.h
server.o: /usr/include/x86_64-linux-gnu/bits/libc-header-start.h
server.o: /usr/include/x86_64-linux-gnu/bits/floatn.h
server.o: /usr/include/x86_64-linux-gnu/bits/floatn-common.h
server.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h
server.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h
server.o: /usr/include/x86_64-linux-gnu/bits/wchar.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/wint_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/locale_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h
server.o: /usr/include/c++/9/exception /usr/include/c++/9/bits/exception.h
server.o: /usr/include/c++/9/bits/char_traits.h
server.o: /usr/include/c++/9/bits/stl_algobase.h
server.o: /usr/include/c++/9/bits/functexcept.h
server.o: /usr/include/c++/9/bits/exception_defines.h
server.o: /usr/include/c++/9/bits/cpp_type_traits.h
server.o: /usr/include/c++/9/ext/type_traits.h
server.o: /usr/include/c++/9/ext/numeric_traits.h
server.o: /usr/include/c++/9/bits/stl_pair.h /usr/include/c++/9/bits/move.h
server.o: /usr/include/c++/9/bits/concept_check.h
server.o: /usr/include/c++/9/bits/stl_iterator_base_types.h
server.o: /usr/include/c++/9/bits/stl_iterator_base_funcs.h
server.o: /usr/include/c++/9/debug/assertions.h
server.o: /usr/include/c++/9/bits/stl_iterator.h
server.o: /usr/include/c++/9/bits/ptr_traits.h
server.o: /usr/include/c++/9/debug/debug.h
server.o: /usr/include/c++/9/bits/predefined_ops.h
server.o: /usr/include/c++/9/bits/localefwd.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++locale.h
server.o: /usr/include/c++/9/clocale /usr/include/locale.h
server.o: /usr/include/x86_64-linux-gnu/bits/locale.h
server.o: /usr/include/c++/9/cctype /usr/include/ctype.h
server.o: /usr/include/x86_64-linux-gnu/bits/types.h
server.o: /usr/include/x86_64-linux-gnu/bits/timesize.h
server.o: /usr/include/x86_64-linux-gnu/bits/typesizes.h
server.o: /usr/include/x86_64-linux-gnu/bits/time64.h
server.o: /usr/include/x86_64-linux-gnu/bits/endian.h
server.o: /usr/include/x86_64-linux-gnu/bits/endianness.h
server.o: /usr/include/c++/9/bits/ios_base.h
server.o: /usr/include/c++/9/ext/atomicity.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h
server.o: /usr/include/pthread.h /usr/include/sched.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/time_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h
server.o: /usr/include/x86_64-linux-gnu/bits/sched.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h
server.o: /usr/include/x86_64-linux-gnu/bits/cpu-set.h /usr/include/time.h
server.o: /usr/include/x86_64-linux-gnu/bits/time.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/clock_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/timer_t.h
server.o: /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h
server.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h
server.o: /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h
server.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h
server.o: /usr/include/x86_64-linux-gnu/bits/struct_mutex.h
server.o: /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h
server.o: /usr/include/x86_64-linux-gnu/bits/setjmp.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/atomic_word.h
server.o: /usr/include/c++/9/bits/locale_classes.h /usr/include/c++/9/string
server.o: /usr/include/c++/9/bits/allocator.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++allocator.h
server.o: /usr/include/c++/9/ext/new_allocator.h /usr/include/c++/9/new
server.o: /usr/include/c++/9/bits/ostream_insert.h
server.o: /usr/include/c++/9/bits/cxxabi_forced.h
server.o: /usr/include/c++/9/bits/stl_function.h
server.o: /usr/include/c++/9/backward/binders.h
server.o: /usr/include/c++/9/bits/range_access.h
server.o: /usr/include/c++/9/bits/basic_string.h
server.o: /usr/include/c++/9/ext/alloc_traits.h
server.o: /usr/include/c++/9/bits/basic_string.tcc
server.o: /usr/include/c++/9/bits/locale_classes.tcc
server.o: /usr/include/c++/9/stdexcept /usr/include/c++/9/streambuf
server.o: /usr/include/c++/9/bits/streambuf.tcc
server.o: /usr/include/c++/9/bits/basic_ios.h
server.o: /usr/include/c++/9/bits/locale_facets.h /usr/include/c++/9/cwctype
server.o: /usr/include/wctype.h
server.o: /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_base.h
server.o: /usr/include/c++/9/bits/streambuf_iterator.h
server.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_inline.h
server.o: /usr/include/c++/9/bits/locale_facets.tcc
server.o: /usr/include/c++/9/bits/basic_ios.tcc
server.o: /usr/include/c++/9/bits/ostream.tcc /usr/include/c++/9/istream
server.o: /usr/include/c++/9/bits/istream.tcc server.h
server.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h
server.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint-gcc.h
server.o: /usr/include/c++/9/random task.h /usr/include/c++/9/deque
server.o: /usr/include/c++/9/bits/stl_construct.h
server.o: /usr/include/c++/9/bits/stl_uninitialized.h
server.o: /usr/include/c++/9/bits/stl_deque.h
server.o: /usr/include/c++/9/bits/deque.tcc /usr/include/c++/9/cassert
server.o: /usr/include/assert.h /usr/include/c++/9/vector
server.o: /usr/include/c++/9/bits/stl_vector.h
server.o: /usr/include/c++/9/bits/stl_bvector.h
server.o: /usr/include/c++/9/bits/vector.tcc event.h
server.o: /usr/include/c++/9/algorithm /usr/include/c++/9/utility
server.o: /usr/include/c++/9/bits/stl_relops.h
server.o: /usr/include/c++/9/bits/stl_algo.h /usr/include/c++/9/cstdlib
server.o: /usr/include/stdlib.h /usr/include/c++/9/bits/std_abs.h
server.o: /usr/include/stdlib.h /usr/include/c++/9/bits/algorithmfwd.h
server.o: /usr/include/c++/9/bits/stl_heap.h
server.o: /usr/include/c++/9/bits/stl_tempbuf.h generator.h
event.o: event.h /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h
event.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint-gcc.h
event.o: /usr/include/c++/9/cassert
event.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++config.h
event.o: /usr/include/x86_64-linux-gnu/c++/9/bits/os_defines.h
event.o: /usr/include/features.h /usr/include/stdc-predef.h
event.o: /usr/include/x86_64-linux-gnu/sys/cdefs.h
event.o: /usr/include/x86_64-linux-gnu/bits/wordsize.h
event.o: /usr/include/x86_64-linux-gnu/bits/long-double.h
event.o: /usr/include/x86_64-linux-gnu/gnu/stubs.h
event.o: /usr/include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h
event.o: /usr/include/assert.h /usr/include/c++/9/algorithm
event.o: /usr/include/c++/9/utility /usr/include/c++/9/bits/stl_relops.h
event.o: /usr/include/c++/9/bits/stl_pair.h /usr/include/c++/9/bits/move.h
event.o: /usr/include/c++/9/bits/concept_check.h
event.o: /usr/include/c++/9/bits/stl_algobase.h
event.o: /usr/include/c++/9/bits/functexcept.h
event.o: /usr/include/c++/9/bits/exception_defines.h
event.o: /usr/include/c++/9/bits/cpp_type_traits.h
event.o: /usr/include/c++/9/ext/type_traits.h
event.o: /usr/include/c++/9/ext/numeric_traits.h
event.o: /usr/include/c++/9/bits/stl_iterator_base_types.h
event.o: /usr/include/c++/9/bits/stl_iterator_base_funcs.h
event.o: /usr/include/c++/9/debug/assertions.h
event.o: /usr/include/c++/9/bits/stl_iterator.h
event.o: /usr/include/c++/9/bits/ptr_traits.h
event.o: /usr/include/c++/9/debug/debug.h
event.o: /usr/include/c++/9/bits/predefined_ops.h
event.o: /usr/include/c++/9/bits/stl_algo.h /usr/include/c++/9/cstdlib
event.o: /usr/include/stdlib.h /usr/include/c++/9/bits/std_abs.h
event.o: /usr/include/stdlib.h /usr/include/c++/9/bits/algorithmfwd.h
event.o: /usr/include/c++/9/bits/stl_heap.h
event.o: /usr/include/c++/9/bits/stl_tempbuf.h
event.o: /usr/include/c++/9/bits/stl_construct.h /usr/include/c++/9/new
event.o: /usr/include/c++/9/exception /usr/include/c++/9/bits/exception.h
event.o: /usr/include/c++/9/ext/alloc_traits.h
event.o: /usr/include/c++/9/bits/allocator.h
event.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++allocator.h
event.o: /usr/include/c++/9/ext/new_allocator.h
event.o: /usr/include/c++/9/bits/memoryfwd.h /usr/include/c++/9/vector
event.o: /usr/include/c++/9/bits/stl_uninitialized.h
event.o: /usr/include/c++/9/bits/stl_vector.h
event.o: /usr/include/c++/9/bits/stl_bvector.h
event.o: /usr/include/c++/9/bits/range_access.h
event.o: /usr/include/c++/9/bits/vector.tcc
generator.o: /usr/include/c++/9/chrono
generator.o: /usr/include/c++/9/bits/c++0x_warning.h
generator.o: /usr/include/c++/9/iostream
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++config.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/os_defines.h
generator.o: /usr/include/features.h /usr/include/stdc-predef.h
generator.o: /usr/include/x86_64-linux-gnu/sys/cdefs.h
generator.o: /usr/include/x86_64-linux-gnu/bits/wordsize.h
generator.o: /usr/include/x86_64-linux-gnu/bits/long-double.h
generator.o: /usr/include/x86_64-linux-gnu/gnu/stubs.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h
generator.o: /usr/include/c++/9/ostream /usr/include/c++/9/ios
generator.o: /usr/include/c++/9/iosfwd /usr/include/c++/9/bits/stringfwd.h
generator.o: /usr/include/c++/9/bits/memoryfwd.h
generator.o: /usr/include/c++/9/bits/postypes.h /usr/include/c++/9/cwchar
generator.o: /usr/include/wchar.h
generator.o: /usr/include/x86_64-linux-gnu/bits/libc-header-start.h
generator.o: /usr/include/x86_64-linux-gnu/bits/floatn.h
generator.o: /usr/include/x86_64-linux-gnu/bits/floatn-common.h
generator.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h
generator.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h
generator.o: /usr/include/x86_64-linux-gnu/bits/wchar.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/wint_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/locale_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h
generator.o: /usr/include/c++/9/exception /usr/include/c++/9/bits/exception.h
generator.o: /usr/include/c++/9/bits/char_traits.h
generator.o: /usr/include/c++/9/bits/stl_algobase.h
generator.o: /usr/include/c++/9/bits/functexcept.h
generator.o: /usr/include/c++/9/bits/exception_defines.h
generator.o: /usr/include/c++/9/bits/cpp_type_traits.h
generator.o: /usr/include/c++/9/ext/type_traits.h
generator.o: /usr/include/c++/9/ext/numeric_traits.h
generator.o: /usr/include/c++/9/bits/stl_pair.h
generator.o: /usr/include/c++/9/bits/move.h
generator.o: /usr/include/c++/9/bits/concept_check.h
generator.o: /usr/include/c++/9/bits/stl_iterator_base_types.h
generator.o: /usr/include/c++/9/bits/stl_iterator_base_funcs.h
generator.o: /usr/include/c++/9/debug/assertions.h
generator.o: /usr/include/c++/9/bits/stl_iterator.h
generator.o: /usr/include/c++/9/bits/ptr_traits.h
generator.o: /usr/include/c++/9/debug/debug.h
generator.o: /usr/include/c++/9/bits/predefined_ops.h
generator.o: /usr/include/c++/9/bits/localefwd.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++locale.h
generator.o: /usr/include/c++/9/clocale /usr/include/locale.h
generator.o: /usr/include/x86_64-linux-gnu/bits/locale.h
generator.o: /usr/include/c++/9/cctype /usr/include/ctype.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types.h
generator.o: /usr/include/x86_64-linux-gnu/bits/timesize.h
generator.o: /usr/include/x86_64-linux-gnu/bits/typesizes.h
generator.o: /usr/include/x86_64-linux-gnu/bits/time64.h
generator.o: /usr/include/x86_64-linux-gnu/bits/endian.h
generator.o: /usr/include/x86_64-linux-gnu/bits/endianness.h
generator.o: /usr/include/c++/9/bits/ios_base.h
generator.o: /usr/include/c++/9/ext/atomicity.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h
generator.o: /usr/include/pthread.h /usr/include/sched.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/time_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h
generator.o: /usr/include/x86_64-linux-gnu/bits/sched.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h
generator.o: /usr/include/x86_64-linux-gnu/bits/cpu-set.h /usr/include/time.h
generator.o: /usr/include/x86_64-linux-gnu/bits/time.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/clock_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/timer_t.h
generator.o: /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h
generator.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h
generator.o: /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h
generator.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h
generator.o: /usr/include/x86_64-linux-gnu/bits/struct_mutex.h
generator.o: /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h
generator.o: /usr/include/x86_64-linux-gnu/bits/setjmp.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/atomic_word.h
generator.o: /usr/include/c++/9/bits/locale_classes.h
generator.o: /usr/include/c++/9/string /usr/include/c++/9/bits/allocator.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++allocator.h
generator.o: /usr/include/c++/9/ext/new_allocator.h /usr/include/c++/9/new
generator.o: /usr/include/c++/9/bits/ostream_insert.h
generator.o: /usr/include/c++/9/bits/cxxabi_forced.h
generator.o: /usr/include/c++/9/bits/stl_function.h
generator.o: /usr/include/c++/9/backward/binders.h
generator.o: /usr/include/c++/9/bits/range_access.h
generator.o: /usr/include/c++/9/bits/basic_string.h
generator.o: /usr/include/c++/9/ext/alloc_traits.h
generator.o: /usr/include/c++/9/bits/basic_string.tcc
generator.o: /usr/include/c++/9/bits/locale_classes.tcc
generator.o: /usr/include/c++/9/stdexcept /usr/include/c++/9/streambuf
generator.o: /usr/include/c++/9/bits/streambuf.tcc
generator.o: /usr/include/c++/9/bits/basic_ios.h
generator.o: /usr/include/c++/9/bits/locale_facets.h
generator.o: /usr/include/c++/9/cwctype /usr/include/wctype.h
generator.o: /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_base.h
generator.o: /usr/include/c++/9/bits/streambuf_iterator.h
generator.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_inline.h
generator.o: /usr/include/c++/9/bits/locale_facets.tcc
generator.o: /usr/include/c++/9/bits/basic_ios.tcc
generator.o: /usr/include/c++/9/bits/ostream.tcc /usr/include/c++/9/istream
generator.o: /usr/include/c++/9/bits/istream.tcc server.h
generator.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h
generator.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint-gcc.h
generator.o: /usr/include/c++/9/random task.h /usr/include/c++/9/deque
generator.o: /usr/include/c++/9/bits/stl_construct.h
generator.o: /usr/include/c++/9/bits/stl_uninitialized.h
generator.o: /usr/include/c++/9/bits/stl_deque.h
generator.o: /usr/include/c++/9/bits/deque.tcc /usr/include/c++/9/cassert
generator.o: /usr/include/assert.h /usr/include/c++/9/vector
generator.o: /usr/include/c++/9/bits/stl_vector.h
generator.o: /usr/include/c++/9/bits/stl_bvector.h
generator.o: /usr/include/c++/9/bits/vector.tcc event.h
generator.o: /usr/include/c++/9/algorithm /usr/include/c++/9/utility
generator.o: /usr/include/c++/9/bits/stl_relops.h
generator.o: /usr/include/c++/9/bits/stl_algo.h /usr/include/c++/9/cstdlib
generator.o: /usr/include/stdlib.h /usr/include/c++/9/bits/std_abs.h
generator.o: /usr/include/stdlib.h /usr/include/c++/9/bits/algorithmfwd.h
generator.o: /usr/include/c++/9/bits/stl_heap.h
generator.o: /usr/include/c++/9/bits/stl_tempbuf.h generator.h
task.o: task.h /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint.h
task.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdint-gcc.h
task.o: /usr/include/c++/9/deque /usr/include/c++/9/bits/stl_algobase.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++config.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/os_defines.h
task.o: /usr/include/features.h /usr/include/stdc-predef.h
task.o: /usr/include/x86_64-linux-gnu/sys/cdefs.h
task.o: /usr/include/x86_64-linux-gnu/bits/wordsize.h
task.o: /usr/include/x86_64-linux-gnu/bits/long-double.h
task.o: /usr/include/x86_64-linux-gnu/gnu/stubs.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/cpu_defines.h
task.o: /usr/include/c++/9/bits/functexcept.h
task.o: /usr/include/c++/9/bits/exception_defines.h
task.o: /usr/include/c++/9/bits/cpp_type_traits.h
task.o: /usr/include/c++/9/ext/type_traits.h
task.o: /usr/include/c++/9/ext/numeric_traits.h
task.o: /usr/include/c++/9/bits/stl_pair.h /usr/include/c++/9/bits/move.h
task.o: /usr/include/c++/9/bits/concept_check.h
task.o: /usr/include/c++/9/bits/stl_iterator_base_types.h
task.o: /usr/include/c++/9/bits/stl_iterator_base_funcs.h
task.o: /usr/include/c++/9/debug/assertions.h
task.o: /usr/include/c++/9/bits/stl_iterator.h
task.o: /usr/include/c++/9/bits/ptr_traits.h /usr/include/c++/9/debug/debug.h
task.o: /usr/include/c++/9/bits/predefined_ops.h
task.o: /usr/include/c++/9/bits/allocator.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++allocator.h
task.o: /usr/include/c++/9/ext/new_allocator.h /usr/include/c++/9/new
task.o: /usr/include/c++/9/exception /usr/include/c++/9/bits/exception.h
task.o: /usr/include/c++/9/bits/memoryfwd.h
task.o: /usr/include/c++/9/bits/stl_construct.h
task.o: /usr/include/c++/9/ext/alloc_traits.h
task.o: /usr/include/c++/9/bits/stl_uninitialized.h
task.o: /usr/include/c++/9/bits/stl_deque.h
task.o: /usr/include/c++/9/bits/range_access.h
task.o: /usr/include/c++/9/bits/deque.tcc /usr/include/c++/9/iostream
task.o: /usr/include/c++/9/ostream /usr/include/c++/9/ios
task.o: /usr/include/c++/9/iosfwd /usr/include/c++/9/bits/stringfwd.h
task.o: /usr/include/c++/9/bits/postypes.h /usr/include/c++/9/cwchar
task.o: /usr/include/wchar.h
task.o: /usr/include/x86_64-linux-gnu/bits/libc-header-start.h
task.o: /usr/include/x86_64-linux-gnu/bits/floatn.h
task.o: /usr/include/x86_64-linux-gnu/bits/floatn-common.h
task.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h
task.o: /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h
task.o: /usr/include/x86_64-linux-gnu/bits/wchar.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/wint_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/mbstate_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/locale_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h
task.o: /usr/include/c++/9/bits/char_traits.h
task.o: /usr/include/c++/9/bits/localefwd.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/c++locale.h
task.o: /usr/include/c++/9/clocale /usr/include/locale.h
task.o: /usr/include/x86_64-linux-gnu/bits/locale.h /usr/include/c++/9/cctype
task.o: /usr/include/ctype.h /usr/include/x86_64-linux-gnu/bits/types.h
task.o: /usr/include/x86_64-linux-gnu/bits/timesize.h
task.o: /usr/include/x86_64-linux-gnu/bits/typesizes.h
task.o: /usr/include/x86_64-linux-gnu/bits/time64.h
task.o: /usr/include/x86_64-linux-gnu/bits/endian.h
task.o: /usr/include/x86_64-linux-gnu/bits/endianness.h
task.o: /usr/include/c++/9/bits/ios_base.h /usr/include/c++/9/ext/atomicity.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/gthr-default.h
task.o: /usr/include/pthread.h /usr/include/sched.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/time_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h
task.o: /usr/include/x86_64-linux-gnu/bits/sched.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h
task.o: /usr/include/x86_64-linux-gnu/bits/cpu-set.h /usr/include/time.h
task.o: /usr/include/x86_64-linux-gnu/bits/time.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/clock_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/timer_t.h
task.o: /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h
task.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h
task.o: /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h
task.o: /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h
task.o: /usr/include/x86_64-linux-gnu/bits/struct_mutex.h
task.o: /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h
task.o: /usr/include/x86_64-linux-gnu/bits/setjmp.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/atomic_word.h
task.o: /usr/include/c++/9/bits/locale_classes.h /usr/include/c++/9/string
task.o: /usr/include/c++/9/bits/ostream_insert.h
task.o: /usr/include/c++/9/bits/cxxabi_forced.h
task.o: /usr/include/c++/9/bits/stl_function.h
task.o: /usr/include/c++/9/backward/binders.h
task.o: /usr/include/c++/9/bits/basic_string.h
task.o: /usr/include/c++/9/bits/basic_string.tcc
task.o: /usr/include/c++/9/bits/locale_classes.tcc
task.o: /usr/include/c++/9/stdexcept /usr/include/c++/9/streambuf
task.o: /usr/include/c++/9/bits/streambuf.tcc
task.o: /usr/include/c++/9/bits/basic_ios.h
task.o: /usr/include/c++/9/bits/locale_facets.h /usr/include/c++/9/cwctype
task.o: /usr/include/wctype.h
task.o: /usr/include/x86_64-linux-gnu/bits/wctype-wchar.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_base.h
task.o: /usr/include/c++/9/bits/streambuf_iterator.h
task.o: /usr/include/x86_64-linux-gnu/c++/9/bits/ctype_inline.h
task.o: /usr/include/c++/9/bits/locale_facets.tcc
task.o: /usr/include/c++/9/bits/basic_ios.tcc
task.o: /usr/include/c++/9/bits/ostream.tcc /usr/include/c++/9/istream
task.o: /usr/include/c++/9/bits/istream.tcc /usr/include/c++/9/cassert
task.o: /usr/include/assert.h /usr/include/c++/9/vector
task.o: /usr/include/c++/9/bits/stl_vector.h
task.o: /usr/include/c++/9/bits/stl_bvector.h
task.o: /usr/include/c++/9/bits/vector.tcc server.h /usr/include/c++/9/random
task.o: /usr/include/c++/9/bits/c++0x_warning.h event.h
task.o: /usr/include/c++/9/algorithm /usr/include/c++/9/utility
task.o: /usr/include/c++/9/bits/stl_relops.h
task.o: /usr/include/c++/9/bits/stl_algo.h /usr/include/c++/9/cstdlib
task.o: /usr/include/stdlib.h /usr/include/c++/9/bits/std_abs.h
task.o: /usr/include/stdlib.h /usr/include/c++/9/bits/algorithmfwd.h
task.o: /usr/include/c++/9/bits/stl_heap.h
task.o: /usr/include/c++/9/bits/stl_tempbuf.h generator.h
