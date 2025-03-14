# CMake generated Testfile for 
# Source directory: /home/sword/swordProject/STLite-ACM-2025/priority_queue
# Build directory: /home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pq_one "sh" "-c" "/home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue/pq_one > /tmp/one_out.txt        && diff -u /home/sword/swordProject/STLite-ACM-2025/priority_queue/data/one/answer.txt /tmp/one_out.txt > /tmp/one_diff.txt")
set_tests_properties(pq_one PROPERTIES  _BACKTRACE_TRIPLES "/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;9;add_test;/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;0;")
add_test(pq_two "sh" "-c" "/home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue/pq_two >/tmp/two_out.txt        && diff -u /home/sword/swordProject/STLite-ACM-2025/priority_queue/data/two/answer.txt /tmp/two_out.txt>/tmp/two_diff.txt")
set_tests_properties(pq_two PROPERTIES  _BACKTRACE_TRIPLES "/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;11;add_test;/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;0;")
add_test(pq_three "sh" "-c" "/home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue/pq_three >/tmp/three_out.txt        && diff -u /home/sword/swordProject/STLite-ACM-2025/priority_queue/data/three/answer.txt /tmp/three_out.txt>/tmp/three_diff.txt")
set_tests_properties(pq_three PROPERTIES  _BACKTRACE_TRIPLES "/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;13;add_test;/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;0;")
add_test(pq_four "sh" "-c" "/home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue/pq_four >/tmp/four_out.txt        && diff -u /home/sword/swordProject/STLite-ACM-2025/priority_queue/data/four/answer.txt /tmp/four_out.txt>/tmp/four_diff.txt")
set_tests_properties(pq_four PROPERTIES  _BACKTRACE_TRIPLES "/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;15;add_test;/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;0;")
add_test(pq_five "sh" "-c" "/home/sword/swordProject/STLite-ACM-2025/cmake-build-debug/priority_queue/pq_five >/tmp/five_out.txt        && diff -u /home/sword/swordProject/STLite-ACM-2025/priority_queue/data/five/answer.txt /tmp/five_out.txt>/tmp/five_diff.txt")
set_tests_properties(pq_five PROPERTIES  _BACKTRACE_TRIPLES "/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;17;add_test;/home/sword/swordProject/STLite-ACM-2025/priority_queue/CMakeLists.txt;0;")
