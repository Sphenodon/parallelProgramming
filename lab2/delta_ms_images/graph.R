library(ggplot2)

for (i in 1:4){
  df <- read.csv(paste0("delta_ms_intel_", i, ".csv"), header = FALSE)
  pts <- ggplot(df, aes(x = as.factor(V1), y=V2))+
    geom_boxplot(colour="slateblue",
                 fill="blue",
                 alpha=0.2,
                 outlier.colour="red",
                 outlier.fill="red",
                 outlier.size=1.5)+
    xlab("Array size")+
    ylab("Milliseconds")+
    theme(axis.text.y = element_text(hjust = 1))
  jpeg(paste0("delta_ms_intel_", i, ".jpg"), width = 720, height = 720)
  print(pts)
  dev.off()
}

jpeg("delta_ms_intel_with_gcc1.jpg", width = 720, height = 720)
df_icc_with_par1 <- read.csv("delta_ms_for_par_gcc1.csv", header = FALSE)
df_icc_with_par1$V4 <- read.csv("delta_ms_intel_1.csv", header = FALSE)$V2
ggplot(df_icc_with_par1, aes(x = as.factor(V1), y=V2))+
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

for (i in 1:4){
  df <- read.csv(paste0("delta_ms_clang_", i, ".csv"), header = FALSE)
  pts <- ggplot(df, aes(x = as.factor(V1), y=V2))+
    geom_boxplot(colour="slateblue",
                 fill="blue",
                 alpha=0.2,
                 outlier.colour="red",
                 outlier.fill="red",
                 outlier.size=1.5)+
    xlab("Array size")+
    ylab("Milliseconds")+
    theme(axis.text.y = element_text(hjust = 1))
  jpeg(paste0("delta_ms_clang_", i, ".jpg"), width = 720, height = 720)
  print(pts)
  dev.off()
}

jpeg("delta_ms_clang_with_gcc1.jpg", width = 720, height = 720)
df_clang_with_par1 <- read.csv("delta_ms_for_par_gcc1.csv", header = FALSE)
df_clang_with_par1$V4 <- read.csv("delta_ms_clang_1.csv", header = FALSE)$V2
ggplot(df_clang_with_par1, aes(x = as.factor(V1), y=V2))+
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

jpeg("delta_ms_icc_par.jpg", width = 720, height = 720)
ggplot(read.csv("delta_ms_icc_par.csv", header = FALSE), aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_icc_par_with_gcc1.jpg", width = 720, height = 720)
df_par_icc_with_par1 <- read.csv("delta_ms_for_par_gcc1.csv", header = FALSE)
df_par_icc_with_par1$V4 <- read.csv("delta_ms_icc_par.csv", header = FALSE)$V2
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