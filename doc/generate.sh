set -e

mkdir code_docs
cd code_docs

git clone -b gh-pages https://git@$GH_REPO_REF
cd $GH_REPO_NAME/docs/$TRAVIS_BRANCH

git config --global push.default simple
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

rm -rf *

echo "" > .nojekyll
echo "INPUT = ../../../../../src" >> $DOXYFILE

doxygen $DOXYFILE 2>&1 | tee doxygen.log

if [ -d "html" ] && [ -f "html/index.html" ]; then
    git add --all
    git commit -m "Deploy code docs to GitHub Pages Travis build: ${TRAVIS_BUILD_NUMBER}" -m "Commit: ${TRAVIS_COMMIT}"
    git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
else
    echo '' >&2
    echo 'Warning: no documentation (html) files have been found!' >&2
    echo 'Warning: not going to push the documentation to GitHub!' >&2
    exit 1
fi
