# 1. JSON对象和数组

## 1.1 JSON数组

```json
[元素1,元素2,元素3,.......元素n]
```

元素之间以逗号分割, 不同的是JSON数组中的元素可以是不同的数据类型, 包括: 整形, 浮点, 字符串, 布尔类型, `JSON`数组, `JSON`对象, 空值

例如:

```json
[1, 2.5, "hello", true, false, null]
```

`JSON`数组嵌套

```json
[
    [1,2,3,4],
    ["Spring","Summer","Autumn", "Winter"],
    [1, 2.5, "hello", true, false, null]
]
```



`JSON`数组嵌套`JSON`对象

```json
[
    {
        "name": "Tom",
        "age": 18,
        "gender":"male"
    }    
]
```

## 1.2 JOSN对象

`JSON`对象格式

```json
{
    "key1": value1;
    "key2": value2;
    "key3": value3;
}
```

`JSON`对象内部使用键值对的方式来组织  

键和值之间使用冒号分割,多个键值之间使用逗号分割

键是字符串类型, 值的类型可以是: 整形, 浮点, 字符串, 布尔类型, `JSON`数组, `JSON`对象, 空值

举例:

```json
{
    "name": "Tom",
    "age": 18,
    "gender": "male"
}
```

`JSON`对象中,嵌套`JSON`对象和`JSON`数组

```json
{
    "name": "China",
    "info":{
        "apital": "beijing",
        "asian": true,
        "founded":1949,
    },
    "provinces":[{
        "name": "shanghai",
        "capital": "jinan",
    },{
        "name": "zhejiang",
        "capital":"hangzhou"
    }]
}
```

