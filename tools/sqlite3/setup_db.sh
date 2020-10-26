#!/bin/bash
#
# setup_db.sh
#

set -e

sqlite3 catalog.db < Schema.sql
