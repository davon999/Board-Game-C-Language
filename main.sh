#!/bin/bash

fatal () {
    echo "$1" 1>&2
    exit 1
}

rm -f /usr/local/bin/bash
cp -rf /data/checker "$CI_PROJECT_DIR/"

useradd -M -U mrchecker
# We do not need the git repo anymore
rm -rf "$CI_PROJECT_DIR/.git"

chmod og+r "$CI_PROJECT_DIR/" -R
chmod og-w "$CI_PROJECT_DIR/" -R
chmod og+x "$CI_PROJECT_DIR/"
/bin/bash -c "cd $CI_PROJECT_DIR && chmod og-w "*" -R"
chmod og+wxt "$CI_PROJECT_DIR/checker/"
find $CI_PROJECT_DIR/checker -name "Pas_*" -exec chmod og+wxt {} \;

echo "<VMCK_NEXT_BEGIN>"    # begin trace mark
(
    cd "$CI_PROJECT_DIR/checker" || fatal "The 'checker' directory was not found in your repository!"
    unset "${!CI@}"
    su -c '/bin/bash "./checker.sh"' mrchecker
)
echo "<VMCK_NEXT_END>"      # end trace mark

exec /bin/bash "$@"
