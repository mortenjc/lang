CP=.:selenium/selenium-server-standalone-2.31.0.jar

all: SelScr.class Test.class


SelScr.class: SelScr.java
	javac -cp $(CP) SelScr.java 

Test.class: Test.java
	javac -cp $(CP) Test.java 

runscr: SelScr.class 
	java -cp $(CP) SelScr

runtest: Test.class 
	java -cp $(CP) Test


clean:
	rm -f test.png
	rm -f *.class
	rm -f chromedriver.log
	rm -f screenshots/*


