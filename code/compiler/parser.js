function parser(tokens) {
  let current = 0;
  function walk() {
    let token = tokens[current];
    if (token.type === "number") {
      current++;
      return {
        type: "NumberLiteral",
        value: token.value,
      };
    }
    // when encounter `(`, wrap to function
    if (token.type === "paren" && token.value === "(") {
      token = tokens[++current];
      const expr = {
        type: "CallExpression",
        name: token.value,
        params: [],
      };
      token = tokens[++current];
      while (token.value !== ")") {
        expr.params.push(walk());
        token = tokens[current];
      }
      current++;
      return expr;
    }
    throw new TypeError(`Unknown token: '${token}'`);
  }
  const ast = {
    type: "Program",
    body: [walk()],
  };

  return ast;
}

module.exports = parser;
