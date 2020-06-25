[@decco]
type user = {
  id: string,
  name: string,
  avatar: string,
};

[@decco]
type users = array(user);

module Queries = {
  include ReactQuery.Query({
    type key = string;
    type data = Belt.Result.t(users, Decco.decodeError);
    type vars = unit;
  });

  let endpoint = {j|/api/users|j}->Some;

  let use = () => {
    use(~key=endpoint, (key, _) =>
      Js.Promise.(
        Fetch.fetch(key)
        |> then_(Fetch.Response.json)
        |> then_(json => json->users_decode->resolve)
      )
    );
  };
};