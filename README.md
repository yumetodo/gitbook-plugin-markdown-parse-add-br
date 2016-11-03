# IMPORTANT

**DO NOT USE THIS PACKAGE!**

update gitbook to v3.2.2 or later. And remove this package from package.json and book.json

# gitbook-plugin-markdown-parse-add-br
[![Build Status](https://travis-ci.org/yumetodo/gitbook-plugin-markdown-parse-add-br.svg?branch=master)](https://travis-ci.org/yumetodo/gitbook-plugin-markdown-parse-add-br)

A GitBook plugin to support newline in paragraph. 

## Usage

In ``package.json``

```json
{
  "devDependencies": {
    "gitbook-cli": "^2.1.2",
    "gitbook-plugin-markdown-parse-add-br": "^1.0.3",
  }
}
```

In `` book.json``

```json
{
  "gitbook": ">=3.0.0",
  "plugins": [
    "markdown-parse-add-br"
  ]
}
```

