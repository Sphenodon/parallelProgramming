df_seq = read.csv("delta_ms_for_seq.csv", header = FALSE)
df_par1 = read.csv("delta_ms_for_par1.csv", header = FALSE)
df_par2 = read.csv("delta_ms_for_par2.csv", header = FALSE)
df_par4 = read.csv("delta_ms_for_par4.csv", header = FALSE)
df_par8 = read.csv("delta_ms_for_par8.csv", header = FALSE)
df_seq_clang = read.csv("delta_ms_for_seq_clang.csv", header = FALSE)
df_par_clang = read.csv("delta_ms_for_par_clang.csv", header = FALSE)

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

jpeg("delta_ms_for_par1.jpg", width = 720, height = 720)
ggplot(df_par1, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_par2.jpg", width = 720, height = 720)
ggplot(df_par2, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_par4.jpg", width = 720, height = 720)
ggplot(df_par4, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_par8.jpg", width = 720, height = 720)
ggplot(df_par8, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_seq_clang.jpg", width = 720, height = 720)
ggplot(df_seq_clang, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()

jpeg("delta_ms_for_par_clang.jpg", width = 720, height = 720)
ggplot(df_par_clang, aes(x = as.factor(V1), y=V2))+
  geom_boxplot(colour="slateblue",
               fill="blue",
               alpha=0.2,
               outlier.colour="red",
               outlier.fill="red",
               outlier.size=1.5)+
  xlab("Array size")+
  ylab("Milliseconds")
dev.off()
