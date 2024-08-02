

<?php
session_start();

include $_SERVER['DOCUMENT_ROOT'] . "/db.php";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $userID = $_POST["userID"];
    $nickname = $_POST['nickname'];
    $password = $_POST["password"];
    $passwordCheck = $_POST["passwordCheck"];
    $defaultimg = "";
    $defaultst = "안녕하세요";
    if (empty($userID) || empty($nickname) || empty($password) || empty($passwordCheck)) {
        $error = "모든 필드를 입력하세요.";
    } else if ($password != $passwordCheck) {
        $error = "비밀번호와 비밀번호 확인이 다릅니다!";
    } else {
        // ID 중복 확인
        $checkID = mq("SELECT * FROM opgg.user WHERE userID='$userID'");
        $checkNickname = mq("SELECT * FROM opgg.user WHERE nickname='$nickname'");
        if ($checkID->num_rows > 0) {
            $error = "이미 사용 중인 아이디입니다.";
        }
        else if ($checkNickname->num_rows > 0) {
            $error = "이미 사용 중인 닉네임입니다.";
        }
        else {
            // 사용자 데이터베이스에 삽입
            $sql = "INSERT INTO opgg.user(userID, nickname, password, profileimage, statemessage, point) VALUES ('$userID', '$nickname', '$password', '$defaultimg', '$defaultst', 0)";

            if (mq($sql)) {
                $_SESSION["userID"] = $userID;
                header("Location: welcome.php");
                exit();
            } else {
                $error = "회원가입 중 오류가 발생했습니다.";
            }
        }
    }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Signup Page</title>
    <link rel="stylesheet" type="text/css" href="/css/signup.css" />
</head>
<body style="overflow-x: hidden; overflow-y: hidden;">
<div class="backgroundImage"></div>
    <!--회원가입 헤더-->
    <div class="header">
        <div class="contect">
        Contact us <strong>watergon1126@naver.com</strong> 
        </div>
    </div>
    <div class="header">
        <div class="logo">
            <a href="/"><img src="img/logoW.png" alt="OP.GG" height="32"></a>
        </div>
    </div>
    <div>
        <div class = "hello">
            <div class = "hello_">
                <h1>Opet 멤버 ID를</h1>
                <h1>생성하세요.</h1> </br>
                <h4>동물들을 사랑하는 사람들의 커뮤니티에</h4>
                <h4>오신것을 환영합니다. 대화를 나누기 위해</h4>
                <h4>회원등록을 해주세요.</h4>
            </div>
        </div>
        <div class ="textbox">
            <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
                <label for="userID">아이디</label></br>
                <input type="text" id="userID" name="userID" required><br><br>
                <label for="userID">닉네임</label></br>
                <input type="text" id="nickname" name="nickname" required><br><br>
                <label for="password">비밀번호</label></br>
                <input type="password" id="password" name="password" required><br><br>
                <label for="password">비밀번호확인</label></br>
                <input type="password" id="passwordCheck" name="passwordCheck" required><br><br>
                <button type="submit">가입</button>
                <?php
                    if (isset($error)) {
                        echo "<p style='color:red;'>$error</p>";
                    }
                    ?>
            </form>
        </div>
    </div>
</body>
</html>