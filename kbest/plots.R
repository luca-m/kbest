#!/usr/bin/env Rscript
#
# Result Plotter
#
# Author: luca.mella@studio.unibo.it
#
# Usage :
# ./plots.R <infile> <outfile>
#
# Parameters:
# - infile      CSV file containing the test results
# - outfile     Output file
#

removeOutliers<-function(x, na.rm=TRUE, ...) {
  qnt <- quantile(x, probs=c(.25, .75), na.rm = na.rm, ...)
  H <- 1.5 * IQR(x, na.rm = na.rm)
  y <- x
  y[x < (qnt[1] - H)] <- NA
  y[x > (qnt[2] + H)] <- NA
  return(y)
}

loadDataFromCsv<-function(filepath){
  data<-read.csv(filepath, header = TRUE)
  return(data)
}

plot_time_by_K<-function(data, title){
  #
  # Parameters:
  #     - data frame
  require(lattice)
  require(reshape)
  data2=data.frame( k=as.factor(as.character(data$k)), 
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2=data2[complete.cases(data2),]
  dd=melt(data2, id.vars=c("k"))
  trellis<-bwplot( k ~ value  | variable, data=dd ,
                    xlab="Elapsed time (sec)", 
                    ylab="k-best values recovered", 
                    main=title,
                   panel=function(x,y,...){
                     #panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                   })
  return(trellis)
}

plot_time_by_B<-function(data, title){
  #
  # Parameters:
  #     - data frame
  require(lattice)
  require(reshape)
  data2<-data.frame( b=as.numeric(data$b),#k=as.factor(as.character(data$k)), 
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("b"))
  dd$b<-as.factor(dd$b)
  trellis<-bwplot( b ~ value  | variable, data=dd ,
                   xlab="Elapsed time (sec)", 
                   ylab="B Capacity", 
                   main=title,
                   panel=function(x,y,...){
                     #panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                     #print(x,y)
                     #panel.points(predict.poli(x))
                     #l<-data.frame( xx=as.numeric(levels(x))[x],
                     #              yy=as.numeric(levels(y))[y])
                     #panel.lmline(x,y, lty=4)
                   })
  return(trellis)
}

plot_time_by_N<-function(data, title){
  #
  # Parameters:
  #     - data frame
  require(lattice)
  require(reshape)
  data2<-data.frame( n=as.factor(as.character(data$X..nvar)), 
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("n"))
  trellis<-bwplot( n ~ value  | variable, data=dd ,
                   xlab="Elapsed time (sec)", 
                   ylab="Number of variables", 
                   main=title,
                   panel=function(x,y,...){
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                   })
  return(trellis)
}


args <- commandArgs(T)
if (length(args) >= 1) {
  infile <- args[1]
  ofile <- args[2]
  
  data <- loadDataFromCsv(infile)

  # Output PDF preparation.
  # A2 Paper format
  height <-11.0  #23.4
  width <- 8.5 #16.5
  
  file<-sub(ofile, pattern='.svg', replacement='',ignore.case=TRUE)
  
  f<- paste(file,"-time_by_k",".svg",sep='')
  svg(f, width=width, height=height)
  tr<-plot_time_by_K(data, paste("Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()

  f<-paste(file,"-time_by_n",".svg",sep='')
  svg(f, width=width, height=height)
  tr<-plot_time_by_N(data, paste("Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()
  
  
  f<- paste(file,"-time_by_b",".svg",sep='')
  svg(f, width = width, height = height)
  tr <- plot_time_by_B(data[data$k==max(data$k) & data$X..nvar==max(data$X..nvar),],paste("Computation Time with K=",max(data$k),"and NVAR=",max(data$X..nvar)))
  print(tr, newpage = TRUE)
  dev.off()
  
  
} else {
  cat("ERR: Mandatory parameters missing!\n")
  cat("Usage :\n ./plots.R <infile> <outfile>\n")
  cat("Params:\n")
  cat(" <infile> Input CSV file containing the test results\n")
  cat(" <outfile> Output file\n")
}
