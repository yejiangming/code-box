package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"strconv"
	"strings"
)

func main() {
	record := `
               id: 35
			 name: "小汪汪"
      		  age: 5
	`
	jsonStr, err := record2json(record)
	if err != nil {
		panic(err)
	}
	fmt.Println(jsonStr)
}

type FieldItem struct {
	Key   string
	Value interface{}
}

func record2json(record string) (string, error) {

	var fields []FieldItem

	lines := strings.Split(record, "\n")
	for _, line := range lines {
		line = strings.TrimSpace(line)
		if line == "" {
			continue
		}

		kv := strings.Split(line, ":")
		if len(kv) < 2 {
			continue
		}
		key := strings.TrimSpace(kv[0])
		kv[1] = strings.TrimSpace(kv[1])
		value := strings.Join(kv[1:], ":")

		vI, err := strconv.ParseInt(value, 10, 64)
		if err == nil {
			fields = append(fields, FieldItem{Key: key, Value: vI})
			continue
		}

		vF, err := strconv.ParseFloat(value, 64)
		if err == nil {
			fields = append(fields, FieldItem{Key: key, Value: vF})
			continue
		}

		fields = append(fields, FieldItem{Key: key, Value: value})
	}

	var buffer bytes.Buffer
	buffer.WriteString("{")
	for i, field := range fields {
		jsonValue, _ := json.Marshal(field.Value)
		buffer.WriteString(fmt.Sprintf("\"%s\":%s", field.Key, jsonValue))
		if i < len(fields)-1 {
			buffer.WriteString(",")
		}
	}
	buffer.WriteString("}")
	return buffer.String(), nil
}
