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