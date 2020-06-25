[@decco]
type comment = {
  postId: string,
  body: string,
  authorId: string,
};

[@decco]
type comments = array(comment);

module Queries = {
  include ReactQuery.QuerySet({
    type key = string;
    type data = Belt.Result.t(comments, Decco.decodeError);
    type queryVars = unit;
    type mutateVars = comment;
  });

  let getEndpoint = postId => {j|/api/comments/?postId=$postId|j};

  let use = (postId: option(string)) => {
    Query.use(
      ~key=postId->Belt.Option.map(postId => postId->getEndpoint), (key, _) =>
      Js.Promise.(
        Fetch.fetch(key)
        |> then_(Fetch.Response.json)
        |> then_(json => json->comments_decode->resolve)
      )
    );
  };

  let useCreate = () => {
    Mutation.use(comment =>
      Js.Promise.(
        Fetch.fetchWithInit(
          "/api/comments",
          Fetch.RequestInit.make(
            ~method_=Post,
            ~body=
              Fetch.BodyInit.make(comment->comment_encode->Js.Json.stringify),
            (),
          ),
        )
        |> then_(Fetch.Response.json)
        |> then_(json => json->comments_decode->resolve)
      )
    );
  };
};