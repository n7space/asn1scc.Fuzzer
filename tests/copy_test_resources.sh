#!/bin/bash
python -c "
import glob
import shutil
for r in glob.glob(r\"${TESTDIR}/$(basename ${TESTFILE} .t)/*\"):
  shutil.copy(r, r\".\")"
