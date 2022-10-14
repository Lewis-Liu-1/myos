clean:
	rm -f *.img *.bin *.o *.out
	find . -maxdepth 3 -name '*.bin' -delete
	find . -maxdepth 3 -name '*.img' -delete
	find . -maxdepth 3 -name '*.sys' -delete
	find . -maxdepth 3 -name '*.o' -delete
	find . -maxdepth 3 -name '*.out' -delete
	find . -maxdepth 3 -name '*.iso' -delete
	
