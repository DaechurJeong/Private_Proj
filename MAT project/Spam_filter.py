import os
import glob
import shutil
import nltk
import pandas as pd
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from keras.preprocessing.text import Tokenizer

# Sample trainning datas path
ham_files = glob.glob("mails\\trainning_ham\\*")
spam_files = glob.glob("mails\\trainning_spam\\*")
test_ham_files = glob.glob("mails\\testing_all\\testing_ham\\*")
test_spam_files = glob.glob("mails\\testing_all\\testing_spam\\*")
test_all_files = glob.glob("mails\\testing_all\\all\\*")
#splitting files
#num = 1
#for name in files:
#     #if num modula 0 when divide by 4:
#    if num % 4 is 0:
#        #go to test
#        src = name
#        dst = test_dest
#        shutil.move(name, dst)
#    else:
#        #go to trainning
#        src = name
#        dst = train_dest
#        shutil.move(name, dst)  
#    num += 1

def calculateProb(organized_data, count):
    prob_list = {}
    for key, value in organized_data.items():
        prob = (1 + organized_data[key]) / (2 + count)
        #print(prob)
    return prob_list

def organize_datas(str):
    tokenizer = Tokenizer()
    tokenizer.fit_on_texts(str)
    sequences_1 = tokenizer.texts_to_sequences(str)
    word_index_1 = tokenizer.word_counts
    newDictionary={}
    # sort dictionary, bigger value first
    sortedList=sorted(word_index_1.values(), reverse=True)
    for sortedKey in sortedList:
        for key, value in word_index_1.items():
            if value==sortedKey:
                newDictionary[key]=value
    print(newDictionary)
    return newDictionary

def read_folder(path):
    subjectList = []
    count = 0
    for name in path:
        start_read = None
        with open(name) as f:
            # number of mail
           count += 1
           data = f.readlines()
           for line in data:
               # only for subject lines
               if "Subject" in line:
                   # skip stopwords like articles (the, an, a, on ... etc)
                   stop_words = set(stopwords.words('english'))
                   words = nltk.word_tokenize(line)
                   words = [word.lower() for word in words if word.isalpha()]
                   newLine = ""
                   for alpha in words:
                       newLine += (alpha + ' ')
                   # skip all punctuations
                   word_tokens = word_tokenize(newLine)
                   filtered_sentence = [w for w in word_tokens if not w in stop_words] 
                   filtered_sentence = []
                   # append all words to the list again
                   for w in word_tokens: 
                       if w not in stop_words: 
                           filtered_sentence.append(w) 
                   subjectList.append(filtered_sentence)
                   break
    return subjectList, count
spam_filter_data, spam_num = read_folder(spam_files)
ham_filter_data, ham_num = read_folder(ham_files)
organized_data_spam = organize_datas(spam_filter_data)
organized_data_ham = organize_datas(ham_filter_data)
test_ham_filter_data, test_ham_num = read_folder(test_ham_files)
test_spam_filter_data, test_spam_num = read_folder(test_spam_files)
prob_ham = ham_num/(ham_num+spam_num)
prob_spam = spam_num/(ham_num+spam_num)
ham_predict_spam = 0
spam_predict_ham = 0
spam_predict_spam = 0
ham_predict_ham = 0
predicted_num = 0
spamDictionary={}
hamDicionary={}
spam_newDic={}
ham_newDic={}
for data_test in ham_filter_data:
    prob_word = 0
    for data_trainning in organized_data_ham:
        if data_trainning=="subject":
            continue
        for data in data_test:
            if data==data_trainning:
                if data in organized_data_spam:
                    prob_ham_word = (((organized_data_ham[data]/ham_num) * prob_ham) / 
                    ((organized_data_spam[data]/spam_num) * prob_spam + organized_data_ham[data]/ham_num * prob_ham))
                    hamDicionary[data] = prob_ham_word

for data_test in spam_filter_data:
    prob_word = 0
    for data_trainning in organized_data_spam:
        if data_trainning=="subject":
            continue
        for data in data_test:
            if data==data_trainning:
                if data in organized_data_ham:
                    prob_word = (((organized_data_spam[data]/spam_num) * prob_spam) / 
                    ((organized_data_spam[data]/spam_num) * prob_spam + organized_data_ham[data]/ham_num * prob_ham))
                    spamDictionary[data] = prob_word

for data in test_ham_filter_data: # data = each subjects
    X_vec = {}
    for data_test in spamDictionary: # data_test = word from trainning
        if data_test in data: # word is in subject?
            X_vec[data_test]=1
        else:
            X_vec[data_test]=0
    for data_test in hamDicionary:
        if data_test in data: #  word is in subject?
            X_vec[data_test]=1
        else:
            X_vec[data_test]=0
    Numerator = 1
    Denominator = 1
    for x in X_vec:
        if X_vec[x]==0:
            Numerator *= (1-organized_data_spam[x]/spam_num)
            Denominator *= (1-organized_data_ham[x]/ham_num)
        else:
            Numerator *= organized_data_spam[x]/spam_num
            Denominator *= organized_data_ham[x]/ham_num
    prob = Numerator*prob_spam / (Numerator*prob_spam + Denominator*prob_ham)
    if prob >= 0.5:
        ham_predict_spam += 1
    else:
        ham_predict_ham += 1
for data in test_spam_filter_data: # data = each subjects
    X_vec = {}
    for data_test in spamDictionary: # data_test = word from trainning
        if data_test in data: # word is in subject?
            X_vec[data_test]=1
        else:
            X_vec[data_test]=0
    for data_test in hamDicionary:
        if data_test in data: #  word is in subject?
            X_vec[data_test]=1
        else:
            X_vec[data_test]=0
    Numerator = 1
    Denominator = 1
    for x in X_vec:
        if X_vec[x]==0:
            Numerator *= (1-organized_data_spam[x]/spam_num)
            Denominator *= (1-organized_data_ham[x]/ham_num)
        else:
            Numerator *= organized_data_spam[x]/spam_num
            Denominator *= organized_data_ham[x]/ham_num
    prob = Numerator*prob_spam / (Numerator*prob_spam + Denominator*prob_ham)
    if prob >= 0.5:
        spam_predict_spam += 1
    else:
        spam_predict_ham += 1
# accuracy
print((spam_predict_spam + ham_predict_ham) / (test_spam_num + test_ham_num))
# precision
print((spam_predict_spam) / (ham_predict_spam + spam_predict_spam))
# recall
print(spam_predict_spam / test_spam_num)
print(test_spam_num)