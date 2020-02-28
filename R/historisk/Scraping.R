#
#
#pacman::p_load('')

rawdata = as.data.frame(read.csv('CARL_B.csv', header=T, sep=";"))
head(rawdata)

data$Date <- as.Date(data$Date)
data$Bid <- as.numeric(data$Bid)
data$Ask <- as.numeric(data$Ask)
data$Opening.price <- as.numeric(data$Opening.price)
data$Closing.price <- as.numeric(data$Closing.price)
data$High.price <- as.numeric(data$High.price)
data$Low.price <- as.numeric(data$Low.price)
data$Average.price <- as.numeric(data$Average.price)
data$Total.volume <- as.numeric(data$Total.volume)
data$Turnover <- as.numeric(data$Turnover)
data$Trades <- as.numeric(data$Trades)
str(data)
# Remove last row due to extra ":"
data = rawdata[,-12]
summary(data)

head(data)


#data$Closing.price > data$Opening.price

hist(data$Closing.price, 
     axes=T,
     main = "Trades")


# Clear 
dev.off()
rm(list = ls())
