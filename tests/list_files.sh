#!/bin/bash
python -c "
from __future__ import print_function
import os
for r in sorted(os.listdir(r\"$1\")):
  print(r)"
