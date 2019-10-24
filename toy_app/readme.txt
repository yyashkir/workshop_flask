Required files:	toy.exe 
				toy_command
				chart.in
Image processing program gnuplot must be accessible.
 				
Interface must display form using "toy_command" which must be 
editable
				
file toy_command is text file:
	range		20
	chart_data	chart.in
	chart_file	toy.png
	result		report.res
	
file chart.in is text file:
	x	y
	0	0.1
	0.2	0.3
	0.5	0.25
	0.7	0.05
	1.0	0.5
	
# run application (from interface):
> toy.exe toy_command

application reads:
		toy_command
		chart.in

application produces files:
		toy.png
		report.res
		
Files 
		toy.png
		report.res
must be put on the interface page
		

	


