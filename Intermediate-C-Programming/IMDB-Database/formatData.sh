#! /bin/bash

# Pull lines from mpaa-ratings-reasons.list needed for
#  database and store them in mpaa.txt
cat mpaa-ratings-reasons.list | grep -m 1000000000 -E -a '^MV: *|^RE: *' > mpaa.txt

# Pull lines from movies.list needed for database and
#  store them in movies.txt
cat movies.list | grep -m 1000000000 -E -a '\"' > movies.txt

# Use only lines after and including line #240 for database
#  and store them in actors.txt
tail --lines=+240 actors.list > actors.txt