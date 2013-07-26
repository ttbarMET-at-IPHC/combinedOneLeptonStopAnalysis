#!/usr/bin/python
# vim : set fileencoding=utf-8 :

import sys
import re

print '#####################################'
print '#      MVA logger analyzer          #'
print '#####################################'
print 'Number of arguments:', len(sys.argv), 'arguments.'
if len(sys.argv) == 1 : 
	print 'Need to give the logfile name to be analyzed'
	sys.exit("Unexpected end of program")
print 'The file to be analyze is:', sys.argv[1]
ifilename = sys.argv[1]

# Ouverture du fichier destination
destination = open("out.log", "w")
 


##############################
#Print the ranking information
##############################
# Ouverture du fichier source
source = open(ifilename, "r")
#list_var = list()
while 1:
  line = source.readline()
  if not line: break
  if line.find("anking result (top variable is best ranked)") != -1: 
	while (line.find("Factor") == -1):  
		#print line.rstrip('\n\r').split('         :')[::1]
		print line.rstrip('\n\r')
		#list_var.append(line.rstrip('\n\r').split(':')[1].rstrip(" "))
		#print re.match('\number',line.rstrip('\n\r').split(':')[1].rstrip(" "))
		line = source.readline()
# Fermerture du fichier source
source.close()
#print list_var
##############################

##############################
#Print the ranking information
##############################
# Ouverture du fichier source
source = open(ifilename, "r")
while 1:
  line = source.readline()
  if not line: break
  if line.find("Evaluation results ranked by best signal efficiency and purity") != -1: 
	while (line.find("Dataset:Default") == -1):  
		print line.rstrip('\n\r').split('--- Factory                  :')[1]
		line = source.readline()
# Fermerture du fichier source
source.close()

##############################
#Print the ranking information
##############################
# Ouverture du fichier source
source = open(ifilename, "r")
while 1:
  line = source.readline()
  if not line: break
  if line.find("ound directory for method:") != -1: 
	while (line.find("----------------") == -1):  
		print line.rstrip('\n\r')
		line = source.readline()
# Fermerture du fichier source
source.close()


# Fermeture du fichier destination
destination.close()
 
