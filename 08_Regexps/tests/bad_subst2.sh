#!/bin/bash
if ../esub "fhd" "fhsgdfd\9" "afhdsk" 2>/dev/null; test $? -ne 4; then exit 1; else exit 0; fi
