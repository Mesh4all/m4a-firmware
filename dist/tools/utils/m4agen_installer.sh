: "${BASE:=$(cd $(dirname $0)/../../../; pwd)}"
cd $BASE || exit;

: "${TOOLS:=${BASE}/dist/tools}"

if m4agen > /dev/null 2>&1
then
echo "m4a-template generator it's installed"
else
    echo "m4a-template-generator it's not installed"
    read -p "Do you want to install it? (y/n)" -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        sudo pip install ${TOOLS}/m4a-generator/.
    else
        exit 1;
    fi
fi
