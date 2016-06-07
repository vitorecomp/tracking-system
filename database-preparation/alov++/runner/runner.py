#!/usr/bin/python

import sys
import os
import json
from pprint import pprint

def main():
	if len(sys.argv) < 3:
		print "Erro numero de agumentos insuficientes"
		print "Sao necessarios os sequintes"
		print "Pasta de entrada"
		print "Pasta de saida"
		sys.exit(1)

	json_name  = sys.argv[1] + "database.json"
	with open(json_name) as data_file:
		data = json.load(data_file)

	print data["name"]

	path_in  = sys.argv[1] + "imagedata++"
	path_out  = sys.argv[2]


	dirs = os.listdir(path_in)
	j = 0;


	for i in dirs:

		dirs_videos = os.listdir(path_in + "/" + i)
		for video in dirs_videos:
			j += 1
			video_final  = path_in + "/" + i + "/" + video
			video_out = path_out + data["name"] + str(j) + ".avi"
			video_final = video_final.replace(" ", "\ ")
			video_out = video_out.replace(" ", "\ ")
			commmand = "./release/database_gen " + video_final + " " + video_out
			os.system(commmand)




if __name__ == "__main__":
    main()
