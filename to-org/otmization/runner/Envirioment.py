import os

class Envirioment:
	@staticmethod
	def getAll(basic_path):
		i = 0
		for root, dirs, files in os.walk(basic_path):
			if (i == 0):
				envirioment = dirs
			i +=1
			#print root
			#print files
		return envirioment

	@staticmethod
	def getImages(image_paths):
		i = 0
		for root, dirs, files in os.walk(basic_path):
			if (i == 0):
				envirioment = dirs
			i +=1
			#print root
			#print files
		for image in images
		return envirioment

	@staticmethod
	def getXml(xml_path):
		i = 0
		for root, dirs, files in os.walk(basic_path):
			if (i == 0):
				envirioment = dirs
			i +=1
			#print root
			#print files
		return envirioment
