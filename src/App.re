[@react.component]
let make = () => {
  let (isLogin, setLogin) = React.useState(() => false);
  isLogin ? <PeopleList /> : <Login setLogin />;
};