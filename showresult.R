for( nn in  c(5,10,20,30,40,50)){
  tr <- paste("result1000",nn,sep="_")
  r1000<-read.csv(paste(tr,"txt",sep="."))

  png(paste(tr,"png",sep="."))
  plot(r1000[,1:2],col=r1000[,3])
  dev.off()

}
