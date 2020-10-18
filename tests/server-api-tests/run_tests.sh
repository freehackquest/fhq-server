#!/bin/bash

# pylint **/*.py
# pylint *.py
# pytest-3 -vvrAs tests_leaks.py

pytest-3 -rAs -c env-travis.ini .

# pytest-3 -rAs -c env-docker.ini tests_leaks.py