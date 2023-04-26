#!/bin/bash
rm -f *.gcov
rm -f app.info
rm -rf CoverageReport

GCNOLIST=( $(find . | grep .gcno ) )

if [ ! -z "${GCNOLIST}" ]
then
   for i in `eval "echo {0..$(expr ${#GCNOLIST[@]} - 1)}"`
   do
      echo "GCNO file (${i}): ${GCNOLIST[i]}"
      gcov ${GCNOLIST[i]}
      echo
   done
   unset GCNOLIST
else
   echo "GCNO files absent"
fi
./build_host_ut/unit-test/fresco/UT_Fresco_TestApp
lcov --rc lcov_branch_coverage=1 --directory . --capture --output-file build_host_ut/app.info
lcov --directory ./build_host_ut/ --capture --output-file build_host_ut/code_coverage.info -rc lcov_branch_coverage=1
lcov --rc lcov_branch_coverage=1 --remove build_host_ut/app.info '/usr/*' -o build_host_ut/app_filtered.info
genhtml build_host_ut/app_filtered.info --branch-coverage --output-directory build_host_ut/CoverageReport
