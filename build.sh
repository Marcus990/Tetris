#!/usr/bin/env bash
set -e

# if [ "$#" -lt 4 ]; then
#     echo "Usage: $0 <local_dir> <user@host> <remote_path> <artifact_pattern>"
#     echo "Example: $0 ./project alice@server /home/alice/build 'biquadris'"
#     exit 1
# fi

LOCAL_DIR="./"
REMOTE="j239cai@linux.student.cs.uwaterloo.ca"
REMOTE_PATH="/u5/j239cai/cs246/1259/project/remote_build"
ARTIFACT_PATTERN="biquadris"

REMOTE_DIR="$REMOTE_PATH/$(basename "$LOCAL_DIR")"

echo "Syncing directory to remote..."
rsync -avz --delete "$LOCAL_DIR" "$REMOTE":"$REMOTE_PATH"

echo "Running make on remote..."
ssh "$REMOTE" "cd '$REMOTE_DIR' && make rebuild"

echo "Downloading build artifacts matching pattern: $ARTIFACT_PATTERN"
rsync -avz "$REMOTE":"$REMOTE_DIR/$ARTIFACT_PATTERN" ./ 

echo "Done."
