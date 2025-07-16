setwd("C:/Users/AlecB/Repos/C-PlusPlus/random")

library(readr)
k_steps <- read_csv("kaprekar_steps.csv")

boxplot(k_steps$Steps, main = "Distribution of Steps to 6174", ylab = "Steps")

barplot(table(k_steps$Steps),
        main = "Frequency of Step Counts to Reach 6174",
        xlab = "Steps",
        ylab = "Number of 4-digit numbers",
        col = "lightgreen")

plot(density(k_steps$Steps), main = "Density of Steps to 6174", xlab = "Steps")

k_steps$FirstDigit <- as.numeric(substr(k_steps$Number, 1, 1))
heat_data <- table(k_steps$FirstDigit, k_steps$Steps)
heat_matrix <- as.matrix(heat_data)
heatmap(heat_matrix, Rowv = NA, Colv = NA,
        main = "Kaprekar Steps by First Digit",
        xlab = "Steps to Reach 6174",
        ylab = "First Digit of Number",
        col = heat.colors(256), scale = "none")


