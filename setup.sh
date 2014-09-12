


# Check if we have the current directory in the library paths
# and add it if not.

# (I'd be glad if somebody have a solution for the system to look automatically
# for the library given the full path given at linking stage instead of having to do this)

PWD_=$PWD
TEST=`echo $LD_LIBRARY_PATH | grep $PWD_`
if [[ "$TEST" == "" ]]
then
    echo "Adding PWD to LD_LIBRARY_PATH (to be able to link vectorDictionnary.so)..."
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD_
fi

echo "Sourcing ROOT ..."
cd /opt/sbg/data/data4/cms/aaubin/CMSSW_5_3_2_patch4/src/
cmsenv
cd $PWD_
echo "Done."
