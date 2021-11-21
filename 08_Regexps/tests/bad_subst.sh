#!/bin/bash
if ../esub "fhd" "fhsgdfd\\" "afhdsk" 2>/dev/null; test $? -ne 6; then exit 1; else exit 0; fi
