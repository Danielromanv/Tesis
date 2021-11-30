#/bin/bash

dir="Instances"

it_int=100000


ARGS="${it_int} ${steps}"

instanceList="1 2 3 4 5 6"
seedList="129 137 279 672 195 666 429 762 821 483 999 729 318 349 287 241 476 555 581 573"

resultsDir=Results_${it_int}
rm -rf ${resultsDir}
mkdir ${resultsDir}

for instance in ${instanceList}; do
   sumarizedOut="Out_${instance}"
   rm -rf ${sumarizedOut}
   best=-1000000.0
   for seed in ${seedList}; do
       detailedOut=out_${instance}_${seed}_${it_int}.out
       rm -rf ${detailedOut}
       echo "./MCPB ${instance} ${seed} ${ARGS} > ${detailedOut}"
       ./MCPB ${instance} ${seed} ${ARGS} > ${detailedOut}
       quality=`tail -1 ${detailedOut} |awk -F ' ' '{print $1}'`
       if (( $(echo "${quality} > ${best}" |bc ) )); then
           best=${quality}
       fi
       quality=`tail -1 ${detailedOut}`
       echo "${seed} ${quality}" >> ${sumarizedOut}
       mv ${detailedOut} ${resultsDir}
   done
   echo "best: ${best}" >> ${sumarizedOut}
   mv ${sumarizedOut} ${resultsDir}

done
