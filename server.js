const faker = require("faker")
const _ = require("lodash")

module.exports = () => {

  const authedUser = {
    id: faker.random.uuid(),
    name: "Carlton Lassiter",
    avatar: faker.image.avatar()
  };

  const users = _.times(4, () => ({
    id: faker.random.uuid(),
    name: `${faker.name.firstName()} ${faker.name.lastName()},`,
    avatar: faker.image.avatar()
  }));


  const data = {
    users: [authedUser, ...users],
    posts: [
      { id: 1, title: faker.name.title(), body: faker.lorem.paragraphs(), authorId: authedUser.id }
    ],
    comments: _.times(5, () => ({
      postId: 1,
      body: faker.lorem.sentences(),
      authorId: users[Math.floor(Math.random() * users.length)]
    }))
  }
  return data
}