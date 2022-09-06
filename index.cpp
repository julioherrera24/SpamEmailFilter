#include <iostream>
#include <string>
#include <cassert>
#include <cctype>
using namespace std;

const int MAX_SPAM_EMAIL = 100; //if score exceeds 100, then it will be a spam email
const int MINIMUM_WORDS = 1; //subject and body minimum amount of words
const double MAX_UPPERCASE_SUBJECT_PERCENT = 90;  //if more than 90% of subject words are uppercase
const double MAX_UPPERCASE_BODY_PERCENT = 50;  //if more than 50% of body words are uppercase
const int SUBJECTLINE_UPPERCASE_SCORE = 30; //score that will be added to total score if more than 90% of subject words are uppercase
const int SUBJECTLINE_NONSENSEWORDS_SCORE = 40; //score that will be added to total score if there are more than 3 consecutive consonants in subject
const int SUBJECTLINE_EXCLAMATIONS_SCORE = 20; //score that will be added to total score if there are 3 or more consecutive exclamation points in subject
const int BODY_UPPERCASE_SCORE = 40; //score that will be added to total score if more than 50% of body words are uppercase
const int BODY_SPECIALWORD_SCORE = 5; //score that will be added to total score for each special word we find in body
const double ADJUST_PERCENTAGE = 100; //helps adjust percentage to a double value

string getFirstWord(string text); //returns first word in a text string
string getLastWord(string text); //returns the last word in a text string
string extractWord(string& text); //returns the first word in the string that text is a reference to
bool isUppercase(string text); //returns true if and only if every letter in the string text is uppercase
string makeUppercase(string text); //returns a new string identical to the string text, except that each lowercase letter of text appears as its uppercase equivalent
bool hasManyExclamations(string text); //returns true if and only if the string text contains three consecutive exclamation marks
bool isAVowel(char x); //returns true if a character is a vowel
bool isNonsenseWord(string text); //returns true if and only if the string text contains more than three consecutive consonants

void doUnitTests();
const bool unitTesting = false; //To test the whole program normally, just change the value to false

int main() {
    
    if(unitTesting) {
        doUnitTests();
        return 0;
    }
    
    string subjectLine{};
    string emailBody{};
    int numOfSpamEmails = 0; //keeps track of the number of spam emails
    int numOfLegitEmails = 0; //keeps track of number of legitimate emails
    bool repeatProgram = true; //set a boolean expression to true in order to repeat the program if the user wants to classify another email
    
    do { //to repeat everything in our program atleast once, we will use a do while loop
// *****SUBJECT PORTION***** //
        cout << "Enter the subject line of the email: ";
        getline(cin, subjectLine); //takes the enter information user input for the subject line of the email
        
        
        int totalSpamScore = 0; //to keep track of the spam score; we will initially set the spam score to 0
         //SUBJECT SPAM RULE NUMBER 1
        string subjectReference = subjectLine; //since we will pass by reference and dont want to change the original subjectLine string, we will make a copy of it to use extractWords function
        double totalWordsSubject = 0;
        double totalUppercaseSubject = 0;
        string firstSubjectWord = getFirstWord(subjectReference); //find the first word in the subject line
        if(firstSubjectWord.size() >= MINIMUM_WORDS) { //if subject has atleast one word
            while (subjectReference != "") { //while we continue to have words in our string
                string extractedSubjectWord = extractWord(subjectReference); //need to extract each word in the string to calculate total number of subject words
                totalWordsSubject = totalWordsSubject + 1; //adds one to total words subject for each word in subject string
                if(isUppercase(extractedSubjectWord)) {
                    totalUppercaseSubject = totalUppercaseSubject + 1; //if a word in subject is all uppercase, then we add one for each to total uppercase words
                }
            }
        }
        if(((totalUppercaseSubject / totalWordsSubject) * ADJUST_PERCENTAGE) > MAX_UPPERCASE_SUBJECT_PERCENT) { //checks to see if more than 90% of words in subject are uppercase
            totalSpamScore = totalSpamScore + SUBJECTLINE_UPPERCASE_SCORE; //if its true then we add 30 points to total spam score
        }
        //cerr << "Spam score for Uppercase in Subject is: " << totalSpamScore << endl;
        
        //SUBJECT SPAM RULE NUMBER 2
        string lastSubjectWord = getLastWord(subjectLine); //using our original subjectLine string, we will check to see if the lastWord in the string contains 3 consecutive consonants
        if(isNonsenseWord(lastSubjectWord)) {
            totalSpamScore = totalSpamScore + SUBJECTLINE_NONSENSEWORDS_SCORE; //if the last word does contain 3 consecutive consonants, we will add 40 points to total spam score
        }
        
        //cerr << "Spam score with/without consonants now equal: " << totalSpamScore << endl;
        
        //SUBJECT SPAM RULE NUMBER 3
        if(hasManyExclamations(subjectLine)) {
            totalSpamScore = totalSpamScore + SUBJECTLINE_EXCLAMATIONS_SCORE; //if the subjectLine string contains 3 or more contiguos exclamation points, we will add 20 points to total spam score
        }
        //cerr << "Spam score with/without exclamations now equal: " << totalSpamScore << endl;
        
// *****EMAIL BODY PORTION***** //
        cout << "Enter the body of the email. Press Enter on an empty line to finish." << endl;
        string body{};
        do {
            getline(cin, emailBody);
            body = body + (emailBody + "\n"); //created a string body variable to take the emailBody information user inputs and add a newline string so the user hits the enter key twice after inputting the email body information
        }while(emailBody != "");
        string totalBodyLine = body;
    
        //BODY SPAM RULE NUMBER 4 & 5
        double totalWordsBody = 0;
        double totalUppercaseBody = 0;
        string bodyReference = totalBodyLine; //since we will pass by reference and dont want to change the original totalBodyLine string, we will make a copy of it to use extractWords function
        string extractedBodyWords = extractWord(bodyReference); // we will create a string for the extract words in the email body
    
        while(extractedBodyWords != "") { //while we continue to have words
            totalWordsBody = totalWordsBody + 1; //for each word in body, we add 1 to total body words
            if(isUppercase(extractedBodyWords)) {
            totalUppercaseBody = totalUppercaseBody + 1; //if the extracted word is all uppercase, then we add 1 to total uppercase words in body
            }
            string specialWords = makeUppercase(extractedBodyWords); //to check for special words in the body information, we can use the makeUppercase function for each extracted word
            if(specialWords == "BUY" || specialWords == "CHEAP" || specialWords == "CLICK" || specialWords == "DIPLOMA" || specialWords == "ENLARGE" || specialWords == "FREE" || specialWords == "LONELY" || specialWords == "MONEY" || specialWords == "NOW" || specialWords == "OFFER" || specialWords == "ONLY" || specialWords == "PILLS" || specialWords == "SEX") {
                totalSpamScore = totalSpamScore + BODY_SPECIALWORD_SCORE; //for every extracted word that equals a special word, we add 5 points to total spam score
            }
            extractedBodyWords = extractWord(bodyReference); //we call to the extractWord function to check the next word in the string
        }
        if(totalWordsBody >= MINIMUM_WORDS) { //if the body of the message has at least one word
            if(((totalUppercaseBody / totalWordsBody) * ADJUST_PERCENTAGE) > MAX_UPPERCASE_BODY_PERCENT) { //and if more than 50% of the words in the string are uppercase
            totalSpamScore = totalSpamScore + BODY_UPPERCASE_SCORE; //we will add 40 points to the total spam score
            }
        }
        //cerr << "Spam score with/without body rules now equal: " << totalSpamScore << endl;
    
        if(totalSpamScore > MAX_SPAM_EMAIL) { //if the total spam score points is greater than 100
            cout << "This email is classified as spam, because its spam score is " << totalSpamScore << "." << endl; //email is spam and return the total score
            numOfSpamEmails = numOfSpamEmails + 1; //increase number of spam emails by 1
        }
        else if(totalSpamScore <= MAX_SPAM_EMAIL) { //if the total spam score is less than or equal to 100
            cout << "This email is classified as legitimate, because its spam score is " << totalSpamScore << "." << endl; //email is legitimate and return the total score
            numOfLegitEmails = numOfLegitEmails + 1; //increase number of legitimate emails by 1
        }
    
        
        string answer{}; //string that will represent either y or n
        cout << "Would you like to classify another email (y or n)? ";
        getline(cin, answer);
        if(answer == "y") {
            repeatProgram = true; //if user inputs "y" then the program will repeat since we will set bool repeatProgram to true and will meet the do while expression
        }
        if(answer == "n") {
            repeatProgram = false; //if the user inputs "n" then we will set repeatProgram to false and the program will not repeat since it will not meet the do while expression
            cout << endl; //prints empty line
            cout << "Number of spam messages: " << numOfSpamEmails << endl; //will return the total number of spam emails that were classified as spam
            cout << "Number of legitimate messages: " << numOfLegitEmails << endl; //will return the total number of legitimate emails that we classified as legit
            break; //we will break out of the do while loop
        }
        while(answer != "y" && answer != "n") { //if the user inputs anything besides "y" or "n"
            cerr << "Please enter y or n." << endl; //will print out error message to enter "y" or "n"
            cout << "Would you like to classify another email (y or n)? ";
            getline(cin, answer); //get the users answer again
            if(answer == "y") {
                repeatProgram = true; //if they input "y" after inputting incorrect answer, we will set repeatProgram to true
            }
            else if(answer == "n") {
                repeatProgram = false; //if the user inputs "n" after inputting incorrect answer, we set repeat Program to false
                cout << endl; //printe empty line
                cout << "Number of spam messages: " << numOfSpamEmails << endl;
                cout << "Number of legitimate messages: " << numOfLegitEmails << endl; //we print out the number of spam & legit emails we tested
                break; //we break out of the do while loop
            }
        }
    } while(repeatProgram == true); //if repeatProgram is true, then the program will repeat
    return 0;
}

string getFirstWord(string text) {
    if(text.size() == 0) { //if an empty string is passed in, we will return ""
        return "";
    }
    int start{}; //we create a varibale start to get the position of the start of the first word
    int end{}; //we create a variable end to get the position of the end of the first word
    bool foundWord = false;
    string firstWord{};
    
    int i = 0;
    while(i < text.size()) {
        if(isalpha(text[i]) && !foundWord) { //if we havent found a word and we find the first character, then we set start to i
            start = i;
            foundWord = true; //we then change the foundword variable to true
        }
        if(!isalpha(text[i]) && foundWord) { //if we found a word and the next position does not contain a alphabet letter, then we set that position to end and break from the loop
            end = i;
            break;
        }
        ++i; //check the next position
    }
    if(foundWord && i == text.size()) { //if we found a word and the string ended with the letter, then we set end to the size of the text
        end = i;
    }
    firstWord = text.substr(start, end - start);
    return firstWord; //returns the first word in a string
}

string getLastWord(string text) {
    if(text.size() == 0) {
        return ""; //if an empty string is passed in, we will return ""
    }
    int start{};
    int end{};
    string lastWord{};
    for(int i = 0; i < text.size() - 1; ++i) {
        if(isalpha(text[i])) {
            start = i;
            while(isalpha(text[i])) { //since we want to get the last word in a string, we want to use a while loop in order to ignore the first couple of words that a string may contain
                lastWord = lastWord + text[i];
                ++i; //since we are increasing i in the while loop, we then have to subtract 1 to the for loop, in order to     not pass boundaries
                if(!isalpha(text[i])) {
                    end = i; //gets the position of the final alphabet character in the last word
                }
            }
        }
    }
    lastWord = text.substr(start, end - start);
    return lastWord; //returns the last word in a string
}

string extractWord(string& text) { //we are passing by reference
    if(text.size() == 0) {
        return ""; //if an empty string is passed in, we will return ""
    }
    string extractedWord{};
    int i = 0; //since we are changing the string everytime we call to this function, we know that the beginning of the word will be 0, also since we are getting rid of all non-letters preceding the previous word
    while(i < text.size()) {
        if(isalpha(text[i])) {
            extractedWord = extractedWord + text[i]; //as the same as getFirstWord, we add all of the alphabet characters in the string to create the word that will be extracted
        }
        else if(extractedWord != "") { //once we have a new extracted word, we will break out of the for loop. also if there are no words left(only punctuations) we will break out and not return them
              break;
        }
        ++i;
    }
    text = text.substr(i, text.size() - i);
    return extractedWord; //returns the new extracted word in a string everytime we call to this function
}

bool isUppercase(string text) {
    if(text.size() == 0) {
        return true; //if the string has no letters or size 0, then we return true
    }
    bool allUpper = true; //first set allUpper to true since I figured it would be easier to change it to false if I find a lowercase letter
    for(int i = 0; i < text.size(); ++i) {
        if(isalpha(text[i]) && islower(text[i])) { //if a character in a string is an alphabet letter and if its lowercase then we automatically set allUpper to false
            allUpper = false;
        }
    }
    return allUpper; //we return either true or false
}

string makeUppercase(string text) {
    if(text.size() == 0) {
        return ""; //if an empty string is passed in, we will return ""
    }
    string allUppercase{}; //we create a string variable that will take all uppercase words
    for(int i = 0; i < text.size(); ++i) {
        if(isalpha(text[i]) && islower(text[i])) { //if we find a lowercase letter, then we need to convert it to an uppercase letter
            text[i] = toupper(text[i]);
            allUppercase = allUppercase + text[i]; //adds the new uppercase letter to the string
        }
        else if(isalpha(text[i]) && isupper(text[i])) { //if they are uppercase, no need to change anything, we just add them together
            allUppercase = allUppercase + text[i];
        }
        else if(!isalpha(text[i])) { //if we find anything that is not an alphabet letter, we let leave it alone and add it to the string
            allUppercase = allUppercase + text[i];
        }
    }
    return allUppercase; //returns all uppercase words with non alphabet characters
}

bool hasManyExclamations(string text) {
    if(text.size() < 3 ) {
        return false; //if an empty string is passed in or if the size of the array is less than 3, we will return false since it will not contains 3 consecutive exclamation points
    }
    bool has3consExclamations = false; //we first set it as false because we dont know if the string has 3 consecutive exclamation marks
    for(int i = 0; i < text.size() - 2; ++i) {
        if(text[i] == '!' && text[i+1] == '!' && text[i+2] == '!') { //if 3 consecutive positions have exclamation marks, then we set the variable to true
            has3consExclamations = true;
        }
    }
    return has3consExclamations; //we return either true or false depending on the string that it takes
}

bool isAVowel(char x) { //in order to check if there are more than 3 consonants together, i thought it would be easier to make a function for vowels since there are only 5 vowels
    bool vowel = false; //we first start it as false because we dont know if a character is a vowel or not
    if(x == 'a' || x == 'A' || x == 'e' || x == 'E' || x == 'i' || x == 'I' || x == 'o' || x == 'O' || x == 'u' || x == 'U') {
        vowel = true; //if the character is a vowel, then we return true
    }
    return vowel;
}

bool isNonsenseWord(string text) {
    if(text.size() < 4) {
        return false; //if an empty string is passed in or if the size of the array is less than 4, we will return false since it will not contain more than 3 consonants
    }
    bool moreThan3Consonants = false; //we first start by creating a boolean variable for more than 3 cons and set it to false
    for(int i = 0; i < text.size() - 3; ++i) { //we subtract 3 to text.size because we dont want to pass boundaries
        if((isalpha(text[i]) && !isAVowel(text[i])) && (isalpha(text[i+1]) && !isAVowel(text[i+1])) && (isalpha(text[i+2]) && !isAVowel(text[i+2])) && (isalpha(text[i+3]) && !isAVowel(text[i+3]))) {
            moreThan3Consonants = true; //if we find more than 3 consecutive consonants in alphabetic letters then we set the variable to true,
        }
    }
    return moreThan3Consonants; //we return either true or false depending on the string that it takes
}

void doUnitTests() {
    //getFirstWord Tests
    assert(getFirstWord("hello there") == "hello");
    assert(getFirstWord("Hello MANN") == "Hello");
    assert(getFirstWord("") == "");
    assert(getFirstWord("*310233232###") == "");
    assert(getFirstWord(" ") == "");
    assert(getFirstWord("hi310 hi hi bye ") == "hi");
    assert(getFirstWord("hi") == "hi");
    assert(getFirstWord("A Bee hit me") == "A");
    assert(getFirstWord("helloooo there") == "helloooo");
    assert(getFirstWord("!!Hello, Fred") == "Hello");
    assert(getFirstWord(" $@#%!!") == "");
    
    //getLastWordTests
    assert(getLastWord("hello there") == "there");
    assert(getLastWord("Hello MANN") == "MANN");
    assert(getLastWord("") == "");
    assert(getLastWord("*310233232###") == "");
    assert(getLastWord(" ") == "");
    assert(getLastWord("hi310 hi hi bye ") == "bye");
    assert(getLastWord("greetings, mom, how are you?") == "you");
    assert(getLastWord("MAKE MONEY FAST!!") == "FAST");
    assert(getLastWord("$$$I love MoNeY$$$") == "MoNeY");
    assert(getLastWord("Hows the Weather BROOO!!!???") == "BROOO");
    
    //extractWord Test
    string ews1 = " 310baby puppy";
    assert(extractWord(ews1) == "baby" && ews1 == " puppy");
    assert(extractWord(ews1) == "puppy" && ews1 == "");
    string ews2 = "54321 ";
    assert(extractWord(ews2) == "" && ews2 == "");
    string ews3 = "yo $$$ h";
    assert(extractWord(ews3) == "yo" && ews3 == " $$$ h");
    assert(extractWord(ews3) == "h" && ews3 == "");
    string s = "***AMAZING!*** Do it, now!!";
    assert(extractWord(s) == "AMAZING" && s == "!*** Do it, now!!");
    assert(extractWord(s) == "Do" && s == " it, now!!");
    assert(extractWord(s) == "it" && s == ", now!!");
    assert(extractWord(s) == "now" && s == "!!");
    assert(extractWord(s) == "" && s == "");
    string s2 = "***543210hello 123232there";
    assert(extractWord(s2) == "hello"  &&  s2 == " 123232there");
    assert(extractWord(s2) == "there"  &&  s2 == "");
    assert(extractWord(s2) == ""  &&  s2 == "");
    string s3 = "***sucks !!!TO &&&Suck";
    assert(extractWord(s3) == "sucks"  &&  s3 == " !!!TO &&&Suck");
    assert(extractWord(s3) == "TO"  &&  s3 == " &&&Suck");
    assert(extractWord(s3) == "Suck"  &&  s3 == "");
    assert(extractWord(s3) == ""  &&  s3 == "");
    
    //isUppercase Tests
    assert(isUppercase("WOW!!"));
    assert(isUppercase("JULIOOOO"));
    assert(isUppercase("HELLOOOOO"));
    assert(isUppercase("ARE U NOT ENTERTAINED####312"));
    assert(isUppercase("RESISTANCE"));
    assert(isUppercase("I HAVE THE FORCE"));
    assert(!isUppercase("WoW!!"));
    assert(!isUppercase("JuuLLiiOO"));
    assert(!isUppercase("HeLLooo!!!"));
    assert(!isUppercase("ResiSTANCE"));
    assert(!isUppercase("i have DA force"));
    
    //makeUppercase Tests
    assert(makeUppercase("Earn *big* MONEY at home!!") == "EARN *BIG* MONEY AT HOME!!");
    assert(makeUppercase("my name is Julio h") == "MY NAME IS JULIO H");
    assert(makeUppercase("310 is the city! where im from") == "310 IS THE CITY! WHERE IM FROM");
    assert(makeUppercase("$$$$$$$$o$$$$$$") == "$$$$$$$$O$$$$$$");
    assert(makeUppercase(" ") == " ");
    assert(makeUppercase("yooooo wassuppppp") == "YOOOOO WASSUPPPPP");
    assert(makeUppercase("i love to code") == "I LOVE TO CODE");
    assert(makeUppercase("c++ beats C") == "C++ BEATS C");
    assert(makeUppercase("!!!!!") == "!!!!!");
    assert(makeUppercase("im getting the hang of this stuff") == "IM GETTING THE HANG OF THIS STUFF");
    
    //hasManyExclamations Tests
    assert(!hasManyExclamations(""));
    assert(hasManyExclamations("HELLO!!!"));
    assert(hasManyExclamations("!!!!!!!!!!"));
    assert(hasManyExclamations("YO!! YOO YOO!!!!"));
    assert(!hasManyExclamations("!! HI HI HI !!"));
    assert(!hasManyExclamations("!! !! !! !!"));
    assert(hasManyExclamations("Wow!!!"));
    assert(hasManyExclamations("Congrats!!!! Good luck!!!!"));
    assert(!hasManyExclamations("W!I!N!"));
    assert(!hasManyExclamations("LAKERS!! !!BABY!!"));
     
    //isNonsenseWord Tests
    assert(isNonsenseWord("AGPQrxab"));
    assert(isNonsenseWord("xxxxozzzz"));
    assert(isNonsenseWord("cbsz rocks"));
    assert(isNonsenseWord("lots of Pennys"));
    assert(isNonsenseWord("thatsss ssso crazzzy"));
    assert(!isNonsenseWord("mortgage"));
    assert(!isNonsenseWord("discount prescriptions"));
    assert(!isNonsenseWord("dddadddadddaddda"));
    assert(!isNonsenseWord("hexxx xxxeh"));
    assert(!isNonsenseWord("julio herrera"));
    
    cerr << "All tests succeeded" << endl;
}
