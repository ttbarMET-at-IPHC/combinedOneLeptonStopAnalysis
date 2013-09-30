
# Check if we have the current directory in the library paths
# and add it if not.

# (I'd be glad if somebody have a solution that doesn't involve
# a dictionnary just so that root knows about vector<TLorentzVector> ...)

TEST=`echo $LD_LIBRARY_PATH | grep $PWD`
if [[ "$TEST" == "" ]]
then
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD
fi
