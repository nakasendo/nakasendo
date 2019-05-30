#!/bin/bash
git log --pretty=oneline --grep "SL-[0-9][0-9]" | cut -d ' ' -f 2- > release-notes.txt
