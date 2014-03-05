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
  require(lattice)
  require(reshape)
  data2<-data.frame( k=as.numeric(data$k), 
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("k"))
  dd$k<-as.factor(dd$k)
  dd$value<-as.numeric(dd$value)
  trellis<-bwplot( k ~ value  | variable, data=dd ,
                    xlab="Elapsed time (sec)", 
                    ylab="k-best values recovered", 
                    main=title,
                   panel=function(x,y,...){
                     panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                   })
  return(trellis)
}
plot_time_by_K_fw<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( k=as.factor(data$k), 
                     forward_time=removeOutliers(data$forward_time), 
                     backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  trellis<-bwplot( k ~ forward_time  , data=data2 ,
                   xlab="Elapsed time (sec)", 
                   ylab="k-best values recovered", 
                   main=title,
                   panel=function(x,y,...){
                     panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(data2),v=-1,col="gray")
                   })
  return(trellis)
}
plot_time_by_K_bw<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( k=as.factor(data$k), 
                     forward_time=removeOutliers(data$forward_time), 
                     backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  trellis<-bwplot( k ~ backward_time  , data=data2 ,
                   xlab="Elapsed time (sec)", 
                   ylab="k-best values recovered", 
                   main=title,
                   panel=function(x,y,...){
                     panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(data2),v=-1,col="gray")
                   })
  return(trellis)
}

plot_time_by_B<-function(data, title){
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
                     panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                   })
  return(trellis)
}

plot_time_by_N<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( n=as.numeric(data$X..nvar), 
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("n"))
  dd$n<-as.factor(dd$n)
  trellis<-bwplot( n ~ value  | variable, data=dd ,
                   xlab="Elapsed time (sec)", 
                   ylab="Number of variables", 
                   main=title,
                   panel=function(x,y,...){
                     panel.xyplot(x,y, pch=4,col='gray')
                     panel.bwplot(x,y,...)
                     panel.grid(h=-length(dd),v=-1,col="gray")
                   })
  return(trellis)
}

plot_time_by_NK<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( k=as.numeric(data$k), n=as.numeric(data$X..nvar),
                    forward_time=removeOutliers(data$forward_time), 
                    backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("k","n"))
  dd$k<-as.numeric(dd$k)
  dd$n<-as.numeric(dd$n)
  dd$value<-as.numeric(dd$value)
  trellis<-cloud( value ~ k * n | variable, data=dd , main=title)
  return(trellis)
}

plot_time_by_NB<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( b=as.numeric(data$b), 
                     n=as.numeric(data$X..nvar),
                     forward_time=removeOutliers(data$forward_time), 
                     backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  dd<-melt(data2, id.vars=c("b","n"))
  dd$b<-as.numeric(dd$b)
  dd$n<-as.numeric(dd$n)
  dd$value<-as.numeric(dd$value)
  trellis<-cloud( value ~ b * n | variable, data=dd , main=title)
  return(trellis)
}

plot_time_by_NBK_forward<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( b=as.numeric(data$b), k=as.numeric(data$k), 
                     n=as.numeric(data$X..nvar),
                     forward_time=removeOutliers(data$forward_time), 
                     backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  trellis<-cloud( forward_time ~ b * n | k, data=data2, main=title)
  return(trellis)
}

plot_time_by_NBK_backward<-function(data, title){
  require(lattice)
  require(reshape)
  data2<-data.frame( b=as.numeric(data$b), k=as.numeric(data$k), 
                     n=as.numeric(data$X..nvar),
                     forward_time=removeOutliers(data$forward_time), 
                     backward_time=removeOutliers(data$backward_time))
  data2<-data2[complete.cases(data2),]
  trellis<-cloud( backward_time ~ b * n | k, data=data2, main=title)
  return(trellis)
}

#
# MAIN
#

args <- commandArgs(T)
if (length(args) >= 1) {
  infile <- args[1]
  ofile <- args[2]
  
  data <- loadDataFromCsv(infile)

  # A4 Paper format
  height <-11.0   # 23.4
  width <- 8.5    # 16.5
  
  file<-sub(ofile, pattern='.svg', replacement='',ignore.case=TRUE)
  
  f<- paste(file,"-time_by_k_fw",".svg",sep='')
  svg(f, width=width, height=height)
  tr<-plot_time_by_K_fw(data, paste("Forward Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()
  
  f<- paste(file,"-time_by_k_bw",".svg",sep='')
  svg(f, width=width, height=height)
  tr<-plot_time_by_K_bw(data, paste("Backward Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()

  f<-paste(file,"-time_by_n",".svg",sep='')
  svg(f, width=width, height=height)
  tr<-plot_time_by_N(data, paste("Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()
  
  f<- paste(file,"-time_by_b",".svg",sep='')
  svg(f, width = width, height = height)
  tr <- plot_time_by_B(data[data$k==median(data$k) & data$X..nvar==median(data$X..nvar),],paste("Computation Time with K=",median(data$k),"and NVAR=",median(data$X..nvar)))
  print(tr, newpage = TRUE)
  dev.off()
  
  f<- paste(file,"-time_by_nbk_fw",".svg",sep='')
  svg(f, width = width, height = height)
  tr <- plot_time_by_NBK_forward(data,"Forward Computation Time")
  print(tr, newpage = TRUE)
  dev.off()
  
  f<- paste(file,"-time_by_nbk_bw",".svg",sep='')
  svg(f, width = width, height = height)
  tr <- plot_time_by_NBK_backward(data,"Backward Computation Time")
  print(tr, newpage = TRUE)
  dev.off()
  
  f<- paste(file,"-time_by_b_all",".svg",sep='')
  svg(f, width = 16.5, height = 23.4)
  tr <- plot_time_by_B(data,paste("Computation Time for problem list:",basename(infile)))
  print(tr, newpage = TRUE)
  dev.off()
  
  
} else {
  cat("ERR: Mandatory parameters missing!\n")
  cat("Usage :\n ./plots.R <infile> <outfile>\n")
  cat("Params:\n")
  cat(" <infile> Input CSV file containing the test results\n")
  cat(" <outfile> Output file\n")
}
