@echo OFF

echo --------------------------------------
echo Solve Problem Lists
echo ----------- luca.mella@studio.unibo.it
echo Usage:
echo 	solve_problem_list.bat INDIR OUTDIR
echo Params:
echo 	INDIR       Directory where to search for .lst files containing path of problems
echo 	OUTDIR      Directory where to save output of the computation of problem lists
echo --------------------------------------

set indir=%1
set outdir=%2

for %%f in (%indir%\*.lst) do (
	echo Processing File %%f
	for %%C in ("%%f") do ( set listFile=%%~nxC )
	echo # nvar, b, k, forward_time, backward_time, total_time > tempfile.temp
	FOR %%I IN (50 100 250 500 1000 1500) DO (
		
		for /F "tokens=*" %%A in (%%f) do (
			:: Launch the solver 
		  kbest.exe -p -k %%I %%A >> tempfile.temp
		)
	)
	move tempfile.temp "%outdir%\%listFile%.solved.csv"
)

