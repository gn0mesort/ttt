#!/usr/bin/env python3
# @file generate_coverage.py
# @brief Test coverage report generation script.
# @author Alexander Rothman <gnomesort@megate.ch>
# @date 2024
# @copyright AGPL-3.0+
import os
import sys
import subprocess
import datetime

def main():
    gcov = os.environ.get("GCOV", "gcov")
    time = datetime.datetime.now().replace(microsecond=0).isoformat()
    file = f"{time}.lcov.info"
    subprocess.run([ "lcov", "--capture", "--exclude=/usr/*", "--exclude=*/tests/*", f"--gcov-tool={gcov}",
                     f"--directory={sys.argv[1]}", f"--output-file={file}" ], check=True)
    subprocess.run([ "genhtml", f"{file}", f"--output-directory=coverage-{time}" ], check=True)

if __name__ == "__main__":
    main()
