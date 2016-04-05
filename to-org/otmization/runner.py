
import time
import sys
sys.path.insert(0, './runner/')

from Database import Database
from Runner import Runner
from Envirioment import Envirioment

def main():
	#database = Database()
	#database.start()
	basic_path = "../database/TrainingDataSet/"
	envirioments = Envirioment.getAll(basic_path)


	for enviroment in envirioments:
		images = Envirioment.getImages(envirioment)
		xml_file = Envirioment.getXml(envirioment)
		

	#	ambiente.getFrames();
	#		for frame in ambiente.frame()
	#		database.exist(frame);


	#compileAll()




if __name__ == "__main__":
    main()
