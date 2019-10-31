[@react.component]
let make = () => {
  let (isLogin, setLogin) =
    React.useState(() => {
      let token = Storage.getTokenFromStorage();

      switch (token) {
      | Some(_t) => true
      | None => false
      };
    });
  isLogin ? <PeopleList /> : <Login setLogin />;
};