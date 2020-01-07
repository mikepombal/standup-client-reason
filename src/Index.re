/* Create an InMemoryCache */
[%bs.raw {|require("./reset.css")|}];

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=Client.instance>
    <ApolloHooks.Provider client=Client.instance>
      <App />
    </ApolloHooks.Provider>
  </ReasonApollo.Provider>,
  "app",
);