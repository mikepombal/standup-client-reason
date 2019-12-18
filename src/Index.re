/* Create an InMemoryCache */
[%bs.raw {|require("./reset.css")|}];

let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let authLink = {
  let authHandler = () => {
    let token = Storage.getTokenFromStorage();

    switch (token) {
    | Some(t) => {
        "headers": {
          "Authorization": {j|Bearer $t|j},
        },
      }
    | None => {
        "headers": {
          "Authorization": "",
        },
      }
    };
  };

  ApolloLinks.createContextLink(authHandler);
};

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(
    ~uri="https://standup-server.herokuapp.com/",
    (),
  );

let client =
  ReasonApollo.createApolloClient(
    ~link=ApolloLinks.from([|authLink, httpLink|]),
    ~cache=inMemoryCache,
    (),
  );

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client>
    <ReasonApolloHooks.ApolloProvider client>
      <App />
    </ReasonApolloHooks.ApolloProvider>
  </ReasonApollo.Provider>,
  "app",
);