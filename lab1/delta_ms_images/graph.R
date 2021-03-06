df_seq <- read.csv("delta_ms_for_seq.csv", header = FALSE)
df_clang_no_vectorization <- read.csv("delta_ms_for_clang_no_vectorization.csv", header = FALSE)
df_clang_with_vectorization <- read.csv("delta_ms_for_clang_with_vectorization.csv", header = FALSE)
df_par_icc <- read.csv("delta_ms_for_par_icc.csv", header = FALSE)

library(ggplot2)

jpeg("delta_ms_for_seq.jpg", width = 720, height = 720)
ggplot(df_seq, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

for (i in c(1,2,4,8)){
  df <- read.csv(paste0("delta_ms_for_par_gcc", i, ".csv"), header = FALSE)
  pts <- ggplot(df, aes(x = as.factor(V1), y=V2))+
    geom_boxplot(colour="slateblue",
                 fill="blue",
                 alpha=0.2,
                 outlier.colour="red",
                 outlier.fill="red",
                 outlier.size=1.5)+
    xlab("Array size")+
    ylab("Milliseconds")
  jpeg(paste0("delta_ms_for_par_gcc", i, ".jpg"), width = 720, height = 720)
  print(pts)
  dev.off()
}

jpeg("delta_ms_for_clang_with_vectorization.jpg", width = 720, height = 720)
df_clang_with_vectorization <- read.csv("delta_ms_for_par_gcc1.csv", header = FALSE)
df_clang_with_vectorization$V4 <- read.csv("delta_ms_for_clang_with_vectorization.csv", header = FALSE)$V2
ggplot(df_clang_with_vectorization, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  geom_boxplot(aes(x = as.factor(V1), y=V4),
               colour="red",
               fill="red",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_clang_no_vectorization.jpg", width = 720, height = 720)
ggplot(df_clang_no_vectorization, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_par_icc.jpg", width = 720, height = 720)
df_par_icc_with_par1 <- read.csv("delta_ms_for_par_gcc1.csv", header = FALSE)
df_par_icc_with_par1$V4 <- read.csv("delta_ms_for_par_icc.csv", header = FALSE)$V2
ggplot(df_par_icc_with_par1, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  geom_boxplot(aes(x = as.factor(V1), y=V4),
               colour="red",
               fill="red",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()