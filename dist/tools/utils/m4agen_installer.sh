: "${BASE:=$(cd $(dirname $0)/../../../; pwd)}"
cd $BASE || exit;

: "${TOOLS:=${BASE}/dist/tools}"

if !(pip > /dev/null 2>&1)
then
    echo "pip isn't installed"
    read -p "Do you want to install it? (y/n)" -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        sudo apt install pip
    else
        exit 1;
    fi
fi

if m4agen > /dev/null 2>&1
then
echo "m4a-template generator it's installed"
else
    echo "m4a-template-generator it's not installed"
    read -p "Do you want to install it? (y/n)" -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        sudo pip install ${TOOLS}/template_generator/.
    else
        exit 1;
    fi
fi
