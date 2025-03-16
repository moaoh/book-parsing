#!/bin/bash

# 실행할 C++ 파일
SRC_FILE="main.cpp"
OUTPUT_FILE="out_json"

# 입력 인자 확인 (파일명이 없으면 오류 발생)
if [ $# -eq 0 ]; then
    echo "❌ 오류: TSV 파일명을 입력하세요!"
    echo "⚠️  사용법: ./run.sh <파일명.tsv>"
    exit 1
fi

TSV_FILE=$1

# 컴파일
echo "🔧 C++ 코드 컴파일 중..."
g++ -std=c++11 -o $OUTPUT_FILE $SRC_FILE -I/opt/homebrew/include

# 컴파일 성공 여부 확인
if [ $? -ne 0 ]; then
    echo "❌ 컴파일 실패!"
    exit 1
fi
echo "✅ 컴파일 완료: ./$OUTPUT_FILE"

# TSV 파일 존재 확인
if [ ! -f "$TSV_FILE" ]; then
    echo "❌ 오류: 파일 '$TSV_FILE'을 찾을 수 없습니다."
    exit 1
fi

# 프로그램 실행
echo "🚀 TSV -> JSON 변환 시작..."
./$OUTPUT_FILE "$TSV_FILE"

# 결과 확인
if [ $? -eq 0 ]; then
    echo "✅ 변환 완료! books.json 파일이 생성되었습니다."
else
    echo "❌ 변환 중 오류 발생!"
fi