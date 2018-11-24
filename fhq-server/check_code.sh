#!/bin/bash

cppcheck --force src | grep "(error)"

